// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <dt-bindings/interconnect/qcom,kalama.h>
#include <linux/clk.h>
#include <linux/interconnect-provider.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>

#include "icc-camsx-clk.h"
#include "icc-camsx-cpas.h"
#include "icc-camsx.h"

int qcom_icc_camsx_cpas_update_state(struct icc_node *node)
{
	int cnt;
	bool en = false;
	bool needs_update = false;
	struct icc_node *n;
	struct camsx_node *csxnode;
	struct icc_camsx_clk_node *clk_node;
	struct qcom_icc_camsx_provider *sxp = to_qcom_camsx_provider(node->provider);

	if (!node->data)
		return -EINVAL;

	/* check only on q-channel master clocks */
	for (cnt = 0; cnt < sxp->qchan_clks_num; cnt++) {
		if (!strcmp(sxp->qchan_clks[cnt], node->name)) {
			needs_update = true;
			break;
		}
	}

	/* if not bail out */
	if (!needs_update)
		return 0;

	/* we've matched a clock we're interested in,
	 * loop all the provider clocks and check if
	 * the master clocks are enabled
	 */
	for (cnt = 0; cnt < sxp->qchan_clks_num; cnt++) {
		list_for_each_entry(n, &sxp->provider.nodes, node_list) {
			csxnode = n->data;
			clk_node = &csxnode->clock;
			if (!strcmp(sxp->qchan_clks[cnt], n->name))
				en |= clk_node->enabled;
		}
	}

	pr_debug("%s: new state: %d\n", __func__, en);

	return qcom_icc_camsx_cpas_qchan_ctl(node, en);
}

int qcom_icc_camsx_cpas_qchan_ctl(struct icc_node *node, bool power_on)
{
	uint32_t status;
	uint32_t mask, wait_data;
	size_t cnt = 0;
	struct qcom_icc_camsx_provider *qp = to_qcom_camsx_provider(node->provider);
	struct regmap *map = qp->regmap;

	if (!map)
		return -ENODEV;

	if (clk_set_rate(qp->icp_clk, qp->icp_clk_freq))
		return -EBUSY;

	if (clk_prepare_enable(qp->icp_clk))
		return -EBUSY;

	regmap_read(map, CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET, &status);
	if (power_on) {
		/* check if device is already active */
		if (status & CPAS_QCHANNEL_STATUS_QACTIVE)
			return 0;

		/* otherwise, send a request to wake up */
		regmap_write(map, CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET, CPAS_QCHANNEL_CTRL_QREQN);

		/* wait for QACCEPTN in QCHANNEL status*/
		mask = BIT(0);
		wait_data = 1;
	} else {
		if (!(status & CPAS_QCHANNEL_STATUS_QACTIVE))
			return 0;

		/* Clear the quiecience request in QCHANNEL ctrl*/
		regmap_write(map, CPAS0_CAMNOC_QCHANNEL_CTRL_OFFSET, 0);

		/* wait for QACCEPTN and QDENY in QCHANNEL status*/
		mask = CPAS_QCHANNEL_STATUS_QDENY | CPAS_QCHANNEL_STATUS_QACCEPTN;
		wait_data = 0;
	}

	regmap_read(map, CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET, &status);
	while (((status & mask) != wait_data) && (cnt++ < CAM_CPAS_POLL_QH_RETRY_CNT)) {
		usleep_range(CAM_CPAS_POLL_MIN_USECS, CAM_CPAS_POLL_MAX_USECS);
		status = regmap_read(map, CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET, &status);
	}

	/* check if deny bit is set */
	regmap_read(map, CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET, &status);

	if ((status & CPAS_QCHANNEL_STATUS_QDENY) && !power_on)
		return -EBUSY;

	clk_disable(qp->icp_clk);

	return 0;
}

static int icc_camsx_cpas_parse_dt(struct device *dev,
				   struct qcom_icc_camsx_provider *sxp)
{
	int ret;
	int cnt;

	ret = of_property_count_strings(dev->of_node, "qchannel-masters");
	if (ret < 0) {
		pr_warn("No q-channel masters defined\n");
		return ret;
	}

	sxp->qchan_clks_num = ret;

	sxp->qchan_clks = devm_kzalloc(dev,
				       sizeof(char *) *
				       sxp->qchan_clks_num, GFP_KERNEL);
	if (!sxp->qchan_clks)
		return -ENOMEM;

	for (cnt = 0; cnt < sxp->qchan_clks_num; cnt++) {
		ret = of_property_read_string_index(dev->of_node,
						    "qchannel-masters",
						    cnt,
						    &sxp->qchan_clks[cnt]);
		if (ret < 0) {
			pr_warn("Failed reading qchannel master clocks\n");
			return -EINVAL;
		}

		pr_info("Register %s as q-channel control clock\n", sxp->qchan_clks[cnt]);
	}

	return 0;
}

int icc_camsx_cpas_register(struct device *dev,
			    struct qcom_icc_camsx_provider *sxp)
{
	int ret;

	ret = icc_camsx_cpas_parse_dt(dev, sxp);
	if (ret)
		return ret;

	return 0;
}
