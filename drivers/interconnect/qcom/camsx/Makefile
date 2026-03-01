# SPDX-License-Identifier: GPL-2.0-only
# Makefile for Qualcomm CAMSX driver

ECHO=

KBUILD_OPTIONS+= CONFIG_INTERCONNECT_QCOM_CAMSX=m
KBUILD_OPTIONS+= KERNEL_ROOT=$(ROOT_DIR)/$(KERNEL_DIR)
KBUILD_EXTRA_SYMBOLS+= local_module.symvers
KBUILD_EXTMOD := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

all: modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $(@)

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $(@)

modules:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) $(@) $(KBUILD_OPTIONS)
