.PHONY: all

TOPDIR := $(CURDIR)
MKRULES := $(TOPDIR)
MKRULES_URL := https://raw.githubusercontent.com/deadskif/smkl/refs/heads/master/rules.mk

SFRTH2BF_VERSION = 0.0.3

CFLAGS += -O2 -Wall -Wextra
CXXFLAGS += $(CFLAGS)

all: rules.mk
$(MKRULES)/rules.mk:
	wget $(MKRULES_URL) -O $@

MODULES = sbf2c sfrth2bf

include $(MKRULES)/rules.mk
