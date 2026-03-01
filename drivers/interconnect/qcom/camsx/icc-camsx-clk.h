/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __ICC_CAMSX_CLK_H__
#define __ICC_CAMSX_CLK_H__

#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <dt-bindings/interconnect/qcom,icc.h>

#define CAMSX_ICC_IDS_FIRST         5000

struct qcom_icc_camsx_provider;

struct icc_camsx_clk_data {
	const char *name;
	unsigned int id;
	int dest_id;
	bool has_rate;
};

struct icc_camsx_clk_node {
	struct clk *clk;
	bool enabled;
	bool has_rate;
};

int qcom_icc_camsx_clk_set(struct icc_node *src, struct icc_node *dst);
int qcom_icc_camsx_clk_get(struct icc_node *node, u32 *avg, u32 *peak);
int icc_camsx_clk_register(struct device *dev, struct qcom_icc_camsx_provider *sxp);
size_t qcom_icc_camsx_clk_get_count(struct icc_camsx_clk_data *clkdata);

#endif /* __ICC_CAMSX_CLK_H__ */
