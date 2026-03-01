// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <dt-bindings/interconnect/qcom,kalama.h>
#include <linux/device.h>
#include <linux/interconnect-provider.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/suspend.h>

#include "icc-camsx.h"

static const struct regmap_config camsx_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
};

static struct qcom_icc_camsx_desc camsx_icc_cfg = {
	.config = &camsx_regmap_config,
};

static int camsx_probe(struct platform_device *pdev)
{
	int ret;

	ret = qcom_icc_camsx_probe(pdev);
	if (ret)
		dev_err(&pdev->dev, "Failed to register camsx icc provider\n");
	else
		dev_dbg(&pdev->dev, "Registered camsx icc provider\n");

	return ret;
}

static int camsx_remove(struct platform_device *pdev)
{
	return qcom_icc_camsx_remove(pdev);
}

static const struct of_device_id camsx_of_match[] = {
	{ .compatible = "qcom,camsx-icc", .data = &camsx_icc_cfg },
	{ },
};
MODULE_DEVICE_TABLE(of, camsx_of_match);

static struct platform_driver camsx_driver = {
	.probe = camsx_probe,
	.remove = camsx_remove,
	.driver = {
		.name = "camsx",
		.of_match_table = camsx_of_match,
		.sync_state = qcom_icc_camsx_sync_state,
	},
};

static int __init camsx_driver_init(void)
{
	return platform_driver_register(&camsx_driver);
}
core_initcall(camsx_driver_init);

MODULE_DESCRIPTION("Camera/camss bridge driver");
MODULE_LICENSE("GPL");
