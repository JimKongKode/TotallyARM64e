#Jim Kong Kracks
include $(THEOS)/makefiles/common.mk
export TARGET = macosx
export ARCHS = x86_64

TOOL_NAME = TotallyARM64e
TotallyARM64e_FILES = $(wildcard *.c)

include $(THEOS_MAKE_PATH)/tool.mk
