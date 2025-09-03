#
# Makefile for the touchscreen drivers.
#

ccflags-y += -Wall

# Each configuration option enables a list of files.
oplus_bsp_tp_td4377_noflash-y += synaptics_tcm_core.o
oplus_bsp_tp_td4377_noflash-y += synaptics_tcm_device.o
oplus_bsp_tp_td4377_noflash-y += synaptics_tcm_recovery.o
oplus_bsp_tp_td4377_noflash-y += synaptics_tcm_zeroflash.o

obj-$(CONFIG_TOUCHPANEL_SYNAPTICS_TD4377_NOFLASH) += oplus_bsp_tp_td4377_noflash.o
#endif
