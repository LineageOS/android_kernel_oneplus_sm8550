// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/clk.h>
#include <linux/interconnect.h>
#include <linux/interconnect-provider.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <dt-bindings/interconnect/qcom,kalama.h>

#include "icc-camsx-clk.h"
#include "icc-camsx.h"
#include "../icc-debug.h"

static struct icc_camsx_clk_data icc_clocks[] = {
	{
		.name		= "cphy_rx_clk_src",
		.id		= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "csid_csiphy_rx_clk",
		.id		= CAMSX_CAMNOC_CSID_CSIPHY_RX_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy0_clk",
		.id		= CAMSX_CAMNOC_CSIPHY0_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy1_clk",
		.id		= CAMSX_CAMNOC_CSIPHY1_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy2_clk",
		.id		= CAMSX_CAMNOC_CSIPHY2_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy3_clk",
		.id		= CAMSX_CAMNOC_CSIPHY3_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy4_clk",
		.id		= CAMSX_CAMNOC_CSIPHY4_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy5_clk",
		.id		= CAMSX_CAMNOC_CSIPHY5_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy6_clk",
		.id		= CAMSX_CAMNOC_CSIPHY6_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "csiphy7_clk",
		.id		= CAMSX_CAMNOC_CSIPHY7_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife_lite_cphy_rx_clk",
		.id		= CAMSX_CAMNOC_IFE_LITE_CPHY_RX_CLK,
		.dest_id	= CAMSX_CAMNOC_CPHY_RX_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "slow_ahb_clk_src",
		.id		= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "bps_ahb_clk",
		.id		= CAMSX_CAMNOC_BPS_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "core_ahb_clk",
		.id		= CAMSX_CAMNOC_CORE_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "cpas_ahb_clk",
		.id		= CAMSX_CAMNOC_CPAS_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "cre_ahb_clk",
		.id		= CAMSX_CAMNOC_CRE_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "drv_ahb_clk",
		.id		= CAMSX_CAMNOC_DRV_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "icp_ahb_clk",
		.id		= CAMSX_CAMNOC_ICP_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife_lite_ahb_clk",
		.id		= CAMSX_CAMNOC_IFE_LITE_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ipe_nps_ahb_clk",
		.id		= CAMSX_CAMNOC_IPE_NPS_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_SLOW_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "fast_ahb_clk_src",
		.id		= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "bps_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_BPS_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "cpas_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_CPAS_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife0_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_IFE0_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife1_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_IFE1_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife2_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_IFE2_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ipe_nps_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_IPE_NPS_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ipe_pps_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_IPE_PPS_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "sbi_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_SBI_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "sfe0_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_SFE0_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "sfe1_fast_ahb_clk",
		.id		= CAMSX_CAMNOC_SFE1_FAST_AHB_CLK,
		.dest_id	= CAMSX_CAMNOC_FAST_AHB_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "camnoc_axi_clk_src",
		.id		= CAMSX_CAMNOC_AXI_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "camnoc_axi_clk",
		.id		= CAMSX_CAMNOC_AXI_CLK,
		.dest_id	= CAMSX_CAMNOC_AXI_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife_lite_clk_src",
		.id		= CAMSX_CAMNOC_IFE_LITE_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "cpas_ife_lite_clk",
		.id		= CAMSX_CAMNOC_CPAS_IFE_LITE_CLK,
		.dest_id	= CAMSX_CAMNOC_IFE_LITE_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife_lite_clk",
		.id		= CAMSX_CAMNOC_IFE_LITE_CLK,
		.dest_id	= CAMSX_CAMNOC_IFE_LITE_CLK_SRC,
		.has_rate	= false,
	},
	{
		.name		= "ife_lite_csid_clk_src",
		.id		= CAMSX_CAMNOC_IFE_LITE_CSID_CLK_SRC,
		.dest_id	= -1,
		.has_rate	= true,
	},
	{
		.name		= "ife_lite_csid_clk",
		.id		= CAMSX_CAMNOC_IFE_LITE_CSID_CLK,
		.dest_id	= CAMSX_CAMNOC_IFE_LITE_CSID_CLK_SRC,
		.has_rate	= false,
	},
	{ },
};

size_t qcom_icc_camsx_clk_get_count(struct icc_camsx_clk_data *clkdata)
{
	size_t num = 0;

	if (!clkdata)
		return num;

	while (clkdata->name) {
		num++;
		clkdata++;
	}

	return num;
}

int qcom_icc_camsx_clk_set(struct icc_node *src, struct icc_node *dst)
{
	struct camsx_node *csxnode;
	struct icc_camsx_clk_node *qn;
	int ret = 0;

	pr_debug("%s: %s %s\n", __func__, src->name, dst->name);

	if (src->data && dst->data) {
		if (src->data == dst->data) {
			pr_debug("%s: [initialization] Avoid init for clocks\n", __func__);
			return ret;
		}
	}

	/* src is a branch clock (supports only enable/disable) */
	csxnode = src->data;
	qn = &csxnode->clock;
	if (!qn || !qn->clk)
		return 0;

	pr_debug("%s: %s has_rate %d avg %u peak_bw %u\n",
		 __func__, src->name, qn->has_rate, src->avg_bw, src->peak_bw);

	if (!qn->has_rate) {
		if (!src->avg_bw) {
			if (qn->enabled)
				clk_disable_unprepare(qn->clk);

			qn->enabled = false;
			pr_debug("%s %d: %s disable\n", __func__, __LINE__, src->name);
			qcom_icc_camsx_cpas_update_state(src);
			goto dst;
		}

		if (!qn->enabled) {
			ret = clk_prepare_enable(qn->clk);
			if (ret)
				return ret;

			qn->enabled = true;
			pr_debug("%s %d: %s enable\n", __func__, __LINE__, src->name);
			qcom_icc_camsx_cpas_update_state(src);
		}
	}

dst:
	/* dst is an rcg clock (rate to be set) */
	csxnode = dst->data;
	qn = &csxnode->clock;
	if (!qn || !qn->clk)
		return 0;

	pr_debug("%s: %s has_rate %d peak_bw %lld\n",
		 __func__, dst->name, qn->has_rate, dst->peak_bw);

	if (qn->has_rate) {
		pr_debug("%s %d: -------- set %s %lld\n",
			 __func__, __LINE__, dst->name, dst->peak_bw);
		ret = clk_set_rate(qn->clk, dst->peak_bw);
	}

	return ret;
}

int qcom_icc_camsx_clk_get(struct icc_node *node, u32 *avg, u32 *peak)
{
	struct icc_camsx_clk_node *qn;
	struct camsx_node *csxnode;

	csxnode = node->data;
	qn = &csxnode->clock;

	if (!qn || !qn->clk)
		*peak = INT_MAX;
	else
		*peak = clk_get_rate(qn->clk);

	return 0;
}

int icc_camsx_clk_register(struct device *dev,
			   struct qcom_icc_camsx_provider *sxp)
{
	size_t num_clocks;
	struct icc_node *node;
	struct camsx_node *csxnode;
	int ret, i;

	num_clocks = qcom_icc_camsx_clk_get_count(icc_clocks);

	sxp->onecell = devm_kzalloc(dev, struct_size(sxp->onecell, nodes, num_clocks), GFP_KERNEL);
	if (!sxp->onecell)
		return -ENOMEM;

	sxp->onecell->num_nodes = num_clocks;
	sxp->provider.data = sxp->onecell;

	for (i = 0; i < num_clocks; i++) {
		csxnode = devm_kzalloc(dev, sizeof(*csxnode), GFP_KERNEL);
		if (!csxnode)
			return -ENOMEM;

		csxnode->type = NODE_CLK;
		csxnode->clock.has_rate = icc_clocks[i].has_rate;

		csxnode->clock.clk = devm_clk_get(dev, icc_clocks[i].name);
		if (IS_ERR(csxnode->clock.clk)) {
			dev_err(dev, "Can't get the requested clock: %s\n", icc_clocks[i].name);
			return -ENXIO;
		}

		node = icc_node_create(CAMSX_ICC_IDS_FIRST + icc_clocks[i].id);
		if (IS_ERR(node)) {
			ret = PTR_ERR(node);
			goto err;
		}

		node->name = devm_kasprintf(dev, GFP_KERNEL, "%s", icc_clocks[i].name);
		if (!node->name) {
			icc_node_destroy(node->id);
			ret = -ENOMEM;
			goto err;
		}

		node->data = csxnode;
		icc_node_add(node, &sxp->provider);

		if (icc_clocks[i].dest_id >= 0) {
			ret = icc_link_create(node, CAMSX_ICC_IDS_FIRST + icc_clocks[i].dest_id);
			if (ret) {
				pr_err("Can't create link %d -> %d: %d\n",
				       icc_clocks[i].id, icc_clocks[i].dest_id, ret);
				goto err;
			}
		}

		sxp->onecell->nodes[i] = node;
	}

	return 0;

err:
	icc_nodes_remove(&sxp->provider);

	return ret;
}
