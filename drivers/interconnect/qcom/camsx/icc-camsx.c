// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/clk.h>
#include <linux/interconnect.h>
#include <linux/interconnect-provider.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <soc/qcom/socinfo.h>
#include <dt-bindings/interconnect/qcom,kalama.h>

#include "icc-camsx-clk.h"
#include "icc-camsx.h"
#include "../icc-debug.h"

int qcom_icc_camsx_setbw(struct icc_node *src, struct icc_node *dst)
{
	struct camsx_node *qsx = src->data;

	switch (qsx->type) {
	case NODE_CLK:
		qcom_icc_camsx_clk_set(src, dst);
		break;
	default:
		pr_err("%s: Unhandled node type: %d\n", __func__, qsx->type);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_setbw);

int qcom_icc_camsx_getbw(struct icc_node *node, u32 *avg, u32 *peak)
{
	struct camsx_node *qsx = node->data;

	switch (qsx->type) {
	case NODE_CLK:
		qcom_icc_camsx_clk_get(node, avg, peak);
		break;

	default:
		pr_err("%s: Unhandled node type: %d\n", __func__, qsx->type);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_getbw);

struct icc_node *qcom_icc_camsx_xlate(struct of_phandle_args *spec, void *data)
{
	return of_icc_xlate_onecell(spec, data);
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_xlate);

void qcom_icc_camsx_sync_state(struct device *dev)
{
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_sync_state);

int qcom_icc_camsx_configure_qos(struct qcom_icc_camsx_provider *qp)
{
	return 0;
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_configure_qos);

static struct regmap *qcom_icc_camsx_map(struct platform_device *pdev,
					 const struct qcom_icc_camsx_desc *desc)
{
	void __iomem *base;
	struct resource *res;
	struct device *dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return NULL;

	base = devm_ioremap(dev, res->start, resource_size(res));
	if (IS_ERR(base))
		return ERR_CAST(base);

	return devm_regmap_init_mmio(dev, base, desc->config);
}

int qcom_icc_camsx_probe(struct platform_device *pdev)
{
	const struct qcom_icc_camsx_desc *desc;
	struct device *dev = &pdev->dev;
	struct qcom_icc_camsx_provider *sxp;
	struct icc_provider *provider;
	int ret;

	desc = of_device_get_match_data(dev);
	if (!desc)
		return -EINVAL;

	sxp = devm_kzalloc(dev, sizeof(*sxp), GFP_KERNEL);
	if (!sxp)
		return -ENOMEM;

	sxp->regmap = qcom_icc_camsx_map(pdev, desc);
	if (IS_ERR(sxp->regmap))
		return PTR_ERR(sxp->regmap);

	sxp->icp_clk = devm_clk_get(dev, "icp_clk");
	if (IS_ERR(sxp->icp_clk))
		return -ENOENT;

	if (of_property_read_u32(dev->of_node, "icp-clock-frequency", &sxp->icp_clk_freq))
		return -ENOENT;

	sxp->dev = dev;

	provider = &sxp->provider;
	provider->dev = dev;
	provider->set = qcom_icc_camsx_setbw;
	provider->aggregate = icc_std_aggregate;
	provider->xlate = qcom_icc_camsx_xlate;
	provider->data = NULL;
	provider->get_bw = qcom_icc_camsx_getbw;

	ret = icc_provider_add(provider);
	if (ret)
		return ret;

	ret = icc_camsx_clk_register(dev, sxp);
	if (ret) {
		dev_err(dev, "ICC clocks registration failed.\n");
		return ret;
	}

	ret = icc_camsx_cpas_register(dev, sxp);
	if (ret) {
		dev_err(dev, "CPAS registration failed: %d\n", ret);
		return ret;
	}

	platform_set_drvdata(pdev, sxp);

	qcom_icc_debug_register(provider);

	return 0;
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_probe);

int qcom_icc_camsx_remove(struct platform_device *pdev)
{
	struct qcom_icc_camsx_provider *qp = platform_get_drvdata(pdev);

	qcom_icc_debug_unregister(&qp->provider);

	icc_nodes_remove(&qp->provider);

	return icc_provider_del(&qp->provider);
}
EXPORT_SYMBOL_GPL(qcom_icc_camsx_remove);

MODULE_LICENSE("GPL");
