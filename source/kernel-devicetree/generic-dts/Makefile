# SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: GPL-2.0-only

objtree ?= $(srctree)

# Redefine the fixdep command
cmd_and_fixdep =                                                             \
	$(cmd);                                                              \
	$(objtree)/scripts/basic/fixdep $(depfile) $@ '$(make-cmd)' > $(dot-target).cmd;\
	rm -f $(depfile)

include $(oottree)/scripts/Makefile.lib

DTC_CPP_FLAGS += $(EXTRA_CPP_FLAGS)

DTC_CPP_FLAGS += -DLINUX_VERSION=600 -DTEGRA_HOST1X_DT_VERSION=2 -DOS_LINUX

ifdef CONFIG_TEGRA_SYSTEM_TYPE_ACK
ifdef CONFIG_TEGRA_KLEAF_BUILD
SOURCE_TOP := $(ROOT_DIR)
else # !CONFIG_TEGRA_KLEAF_BUILD
ifneq ($(TOP),)
SOURCE_TOP := $(TOP)
else
$(error TOP is not defined)
endif
endif # end of CONFIG_TEGRA_KLEAF_BUILD
else # !CONFIG_TEGRA_SYSTEM_TYPE_ACK
ifneq ($(TEGRA_TOP),)
SOURCE_TOP := $(TEGRA_TOP)
else ifneq ($(TOP),)
SOURCE_TOP := $(TOP)
else
$(error TEGRA_TOP or TOP is not defined)
endif
endif # end of CONFIG_TEGRA_SYSTEM_TYPE_ACK

ifdef CONFIG_TEGRA_KLEAF_BUILD
tegra-dtstree := $(SOURCE_TOP)/external/nvidia_dts/hardware/nvidia
else
tegra-dtstree := $(SOURCE_TOP)/hardware/nvidia
endif

# Get relative path for tegra-dtstree from this makefile
null :=
space :=$(null) $(null)
obj-path := $(obj)
obj-path-from-top := $(subst $(SOURCE_TOP),,$(obj-path))
rel-top-path := $(subst /, $(space)/,$(obj-path-from-top))
rel-top-path := $(patsubst /%,../,$(rel-top-path))
rel-top-path := $(subst $(space),,$(rel-top-path))
ifdef CONFIG_TEGRA_KLEAF_BUILD
tegra-rel-dtstree := $(rel-top-path)/external/nvidia_dts/hardware/nvidia
else
tegra-rel-dtstree := $(rel-top-path)/hardware/nvidia
endif

DTC_INCLUDE :=
# SOC independent common include
DTC_INCLUDE += $(tegra-dtstree)/tegra/nv-public
DTC_INCLUDE += $(tegra-dtstree)/tegra/nv-public/include/kernel
DTC_INCLUDE += $(tegra-dtstree)/tegra/nv-public/include/nvidia-oot
DTC_INCLUDE += $(tegra-dtstree)/tegra/nv-public/include/platforms

# SOC T23X specific common include
DTC_INCLUDE += $(tegra-dtstree)/t23x/nv-public/include/kernel
DTC_INCLUDE += $(tegra-dtstree)/t23x/nv-public/include/nvidia-oot
DTC_INCLUDE += $(tegra-dtstree)/t23x/nv-public/include/platforms
DTC_INCLUDE += $(tegra-dtstree)/t23x/nv-public
DTC_INCLUDE += $(tegra-dtstree)/t264/nv-public/include/kernel-t264
DTC_INCLUDE += $(tegra-dtstree)/t264/nv-public

dtb-y :=
dtbo-y :=

DTB_LIST := $(dtb-y)
DTBO_LIST := $(dtbo-y)
dtb-y :=
fdts := $(tegra-dtstree)/t23x
fdts += $(tegra-dtstree)/t264
kdts := $(fdts)
dts_makefile=$(foreach d,$(wildcard $1*), $(call dts_makefile,$(d)/,$(2)) $(if $(findstring Makefile,$(d)),$(d)))
dts_mfiles = $(call dts_makefile, $(kdts), Makefile)

ifneq ($(dts_mfiles),)
dts-include :=
include $(dts_mfiles)
dtb-y := $(addprefix $(tegra-rel-dtstree)/,$(dtb-y))
dtbo-y := $(addprefix $(tegra-rel-dtstree)/,$(dtbo-y))
ifneq ($(dts-include),)
DTC_INCLUDE += $(addprefix $(tegra-dtstree)/,$(dts-include))
endif
endif

DTB_LIST += $(dtb-y)
DTBO_LIST += $(dtbo-y)
DTB_NEW_RULE_LIST := $(dtb-y)
DTBO_NEW_RULE_LIST := $(dtbo-y)

# Now save DTB_LIST to dtb-y
dtb-y := $(DTB_LIST)
dtbo-y := $(DTBO_LIST)

# Add path of main Makefile to each dtb/dtbo list
DTB_OBJS := $(addprefix $(obj)/,$(DTB_LIST))
DTBO_OBJS := $(addprefix $(obj)/,$(DTBO_LIST))

###
# remove the ../
replace_ddot = $(subst ../,,$(1))

define _define_dtb_rule
$(obj)/$(call replace_ddot,$(1)): $(src)/$(patsubst %.dtb,%.dts,$(1)) FORCE
endef

$(foreach _dtb, $(DTB_NEW_RULE_LIST), $(eval $(call _define_dtb_rule,$(_dtb))))

DTB_OBJS := $(call replace_ddot,$(DTB_OBJS))
DTB_NEW_RULE_LIST := $(addprefix $(obj)/,$(DTB_NEW_RULE_LIST))
DTB_NEW_RULE_LIST := $(call replace_ddot,$(DTB_NEW_RULE_LIST))

$(DTB_NEW_RULE_LIST):
	$(call if_changed_dep,dtc)

define _define_dtbo_rule
$(obj)/$(call replace_ddot,$(1)): $(src)/$(patsubst %.dtbo,%.dts,$(1)) FORCE
endef

$(foreach _dtbo, $(DTBO_NEW_RULE_LIST), $(eval $(call _define_dtbo_rule,$(_dtbo))))

DTBO_OBJS := $(call replace_ddot,$(DTBO_OBJS))
DTBO_NEW_RULE_LIST := $(addprefix $(obj)/,$(DTBO_NEW_RULE_LIST))
DTBO_NEW_RULE_LIST := $(call replace_ddot,$(DTBO_NEW_RULE_LIST))

$(DTBO_NEW_RULE_LIST):
	$(call if_changed_dep,dtc)

DTBS_DTBOS := $(DTB_OBJS)$(DTBO_OBJS)

dtbs: $(DTB_OBJS) $(DTBO_OBJS) FORCE
	if [ ! -d $(obj)/dtbs/ ] ; then \
		mkdir -p $(obj)/dtbs/ ; \
	fi
	if [ ! -z "$(DTBS_DTBOS)" ] ; then \
		cp -u $(DTB_OBJS) $(DTBO_OBJS) $(obj)/dtbs/ ; \
	fi
ifdef CONFIG_TEGRA_KLEAF_BUILD
	if [ -d $(obj)/external/nvidia_dts/hardware/ ] ; then \
		rm -rf $(obj)/external/nvidia_dts/hardware/ ; \
	fi
else
	if [ -d $(obj)/hardware/ ] ; then \
		rm -rf $(obj)/hardware/ ; \
	fi
endif

clean-files := *.dtb *.dtbo *.tmp
