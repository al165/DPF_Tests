#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

# --------------------------------------------------------------
# Project name, used for binaries

NAME = SamplePlayer

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	SamplePlayer.cpp

FILES_UI = \
	SamplePlayerUI.cpp \
	widgets/SimpleButton.cpp \
	widgets/Knob.cpp \
	widgets/HBox.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk


# --------------------------------------------------------------
# Extra Flags
# LINK_FLAGS += --libs=sndfile;
BASE_FLAGS += -I. -I./widgets
LDFLAGS=-lsndfile -L/usr/lib

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_OPENGL),true)

TARGETS += jack

ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_LIBLO),true)
# TARGETS += dssi
endif # HAVE_LIBLO
endif # MACOS_OR_WINDOWS

TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3
TARGETS += clap

endif # HAVE_OPENGL

all: $(TARGETS)
