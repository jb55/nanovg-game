#
# Copyright 2011-2015 Branimir Karadzic. All rights reserved.
# License: http://www.opensource.org/licenses/BSD-2-Clause
#

SILENT?=@

BGFX_PREFIX ?= /usr/local
SHADERC ?= $(BGFX_PREFIX)/bin/shaderc
BGFX_SHARE ?= $(BGFX_PREFIX)/share/bgfx
SHADERC_INCLUDES ?= "$(BGFX_SHARE)/src;$(BGFX_SHARE)/common"

VS_FLAGS = -i $(SHADERC_INCLUDES) --type vertex
FS_FLAGS = -i $(SHADERC_INCLUDES) --type fragment

VS_SOURCES=$(wildcard vs_*.sc)
FS_SOURCES=$(wildcard fs_*.sc)

VS_BIN = $(addsuffix .bin.h, $(basename $(VS_SOURCES)))
FS_BIN = $(addsuffix .bin.h, $(basename $(FS_SOURCES)))

BIN = $(VS_BIN) $(FS_BIN)

SHADER_TMP = $(TEMP)/tmp

vs_%.bin.h : vs_%.sc
	@echo [$(<)]
	$(SILENT) $(SHADERC) $(VS_FLAGS) --platform linux -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_glsl
	@cat $(SHADER_TMP) > $(@)
	-$(SILENT) $(SHADERC) $(VS_FLAGS) --platform windows -p vs_3_0 -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_dx9
	-@cat $(SHADER_TMP) >> $(@)
	-$(SILENT) $(SHADERC) $(VS_FLAGS) --platform windows -p vs_4_0 -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_dx11
	-@cat $(SHADER_TMP) >> $(@)
	-$(SILENT) $(SHADERC) $(VS_FLAGS) --platform ios     -p metal  -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_mtl
	-@cat $(SHADER_TMP) >> $(@)

fs_%.bin.h : fs_%.sc
	@echo [$(<)]
	$(SILENT) $(SHADERC) $(FS_FLAGS) --platform linux                  -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_glsl
	@cat $(SHADER_TMP) > $(@)
	-$(SILENT) $(SHADERC) $(FS_FLAGS) --platform windows -p ps_3_0 -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_dx9
	-@cat $(SHADER_TMP) >> $(@)
	-$(SILENT) $(SHADERC) $(FS_FLAGS) --platform windows -p ps_4_0 -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_dx11
	-@cat $(SHADER_TMP) >> $(@)
	-$(SILENT) $(SHADERC) $(FS_FLAGS) --platform ios     -p metal  -O 3 -f $(<) -o $(SHADER_TMP) --bin2c $(basename $(<))_mtl
	-@cat $(SHADER_TMP) >> $(@)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	@echo Cleaning...
	@-rm -vf $(BIN)

.PHONY: rebuild
rebuild: clean all
