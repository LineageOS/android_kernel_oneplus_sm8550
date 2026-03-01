/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __ICC_CAMSX_CPAS_H__
#define __ICC_CAMSX_CPAS_H__

#include <linux/interconnect-provider.h>

#define CPAS0_CAMNOC_QCHANNEL_CTRL_OFFSET	0x5C
#define CPAS0_CAMNOC_QCHANNEL_STAT_OFFSET	0x60

/* ctrl */
#define CPAS_QCHANNEL_CTRL_QREQN	BIT(0)

/* status */
#define CPAS_QCHANNEL_STATUS_QACCEPTN	BIT(0)
#define CPAS_QCHANNEL_STATUS_QDENY	BIT(1)
#define CPAS_QCHANNEL_STATUS_QACTIVE	BIT(2)

/* Number of times to retry while polling */
#define CAM_CPAS_POLL_QH_RETRY_CNT	50
/* Minimum usecs to sleep while polling */
#define CAM_CPAS_POLL_MIN_USECS		200
/* Maximum usecs to sleep while polling */
#define CAM_CPAS_POLL_MAX_USECS		250

int icc_camsx_cpas_register(struct device *dev,
			    struct qcom_icc_camsx_provider *sxp);
int qcom_icc_camsx_cpas_update_state(struct icc_node *node);
int qcom_icc_camsx_cpas_qchan_ctl(struct icc_node *node, bool power_on);

#endif /* __ICC_CAMSX_CPAS_H__ */
