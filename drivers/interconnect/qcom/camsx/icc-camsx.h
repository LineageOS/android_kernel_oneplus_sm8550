/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __ICC_CAMSX_H__
#define __ICC_CAMSX_H__

#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <dt-bindings/interconnect/qcom,icc.h>

#include "icc-camsx-clk.h"
#include "icc-camsx-cpas.h"

#define to_qcom_camsx_provider(_provider) \
	container_of(_provider, struct qcom_icc_camsx_provider, provider)

enum camsx_node_type {
	NODE_CLK = 0,
	NODE_QCHAN,
	NODE_QOS,
	NODE_MAX,
};

struct camsx_node {
	enum camsx_node_type type;
	union {
		int num_clocks;
		struct icc_camsx_clk_node clock;
	};
};

struct qcom_icc_camsx_provider {
	struct icc_provider provider;
	struct device *dev;
	struct regmap *regmap;
	struct clk *icp_clk;
	u32 icp_clk_freq;
	size_t qchan_clks_num;
	const char **qchan_clks;
	/* keep last */
	struct icc_onecell_data *onecell;
};

struct qcom_icc_camsx_desc {
	const struct regmap_config *config;
};

void qcom_icc_camsx_pre_aggregate(struct icc_node *node);
int qcom_icc_camsx_aggregate(struct icc_node *node, u32 tag,
			     u32 avg_bw, u32 peak_bw,
			     u32 *agg_avg, u32 *agg_peak);
int qcom_icc_camsx_setbw(struct icc_node *src, struct icc_node *dst);
struct icc_node_data *qcom_icc_camsx_xlate_extended(struct of_phandle_args *spec, void *data);
int qcom_icc_camsx_getbw(struct icc_node *node, u32 *avg, u32 *peak);
int qcom_icc_camsx_probe(struct platform_device *pdev);
int qcom_icc_camsx_remove(struct platform_device *pdev);
void qcom_icc_camsx_sync_state(struct device *dev);
int qcom_icc_camsx_configure_qos(struct qcom_icc_camsx_provider *qp);
#endif /* __ICC_CAMSX_H__ */
