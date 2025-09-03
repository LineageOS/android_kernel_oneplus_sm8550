#
# Makefile for the touchscreen drivers.
#

ifeq ($(OPLUS_FEATURE_BSP_DRV_INJECT_TEST),1)
ccflags-y += -DCONFIG_TOUCHPANEL_FAULT_INJECT_ENABLE
$(info OPLUS_FEATURE_BSP_DRV_INJECT_TEST and CONFIG_TOUCHPANEL_FAULT_INJECT_ENABLE is Y)
else ifeq ($(OPLUS_FEATURE_BSP_DRV_VND_INJECT_TEST),1)
ccflags-y += -DCONFIG_TOUCHPANEL_FAULT_INJECT_ENABLE
$(info OPLUS_FEATURE_BSP_DRV_VND_INJECT_TEST and CONFIG_TOUCHPANEL_FAULT_INJECT_ENABLE is Y)
else
$(info CONFIG_TOUCHPANEL_FAULT_INJECT_ENABLE is N)
endif

oplus_bsp_tp_comon-y := util_interface/touch_interfaces.o
oplus_bsp_tp_comon-y += touch_comon_api/touch_comon_api.o
oplus_bsp_tp_comon-y += touchpanel_healthinfo/touchpanel_healthinfo.o
oplus_bsp_tp_comon-y += touchpanel_healthinfo/touchpanel_exception.o
oplus_bsp_tp_comon-y += touchpanel_autotest/touchpanel_autotest.o
oplus_bsp_tp_comon-y += touchpanel_prevention/touchpanel_prevention.o

oplus_bsp_tp_comon-y += touchpanel_common_driver.o
oplus_bsp_tp_comon-y += touchpanel_proc.o
oplus_bsp_tp_comon-y += tp_ioctl.o
oplus_bsp_tp_comon-y += message_list.o
oplus_bsp_tp_comon-y += touch_pen/touch_pen_core.o
oplus_bsp_tp_comon-y += touch_pen/touch_pen_algo.o

obj-$(CONFIG_TOUCHPANEL_OPLUS) += oplus_bsp_tp_comon.o

oplus_bsp_tp_notify-y += touchpanel_notify/touchpanel_event_notify.o
oplus_bsp_tp_comon-y += touchpanel_tui_support/touchpanel_tui_support.o
obj-$(CONFIG_TOUCHPANEL_NOTIFY) += oplus_bsp_tp_notify.o

obj-$(CONFIG_TOUCHPANEL_SYNAPTICS) += Synaptics/
obj-$(CONFIG_TOUCHPANEL_GOODIX) += Goodix/

KBUILD_CFLAGS += -Wframe-larger-than=4096
