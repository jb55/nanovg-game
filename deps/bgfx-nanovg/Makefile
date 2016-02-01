
CFLAGS ?= -O2 -fPIC
PREFIX ?= /usr/local
LIBS = libbgfx-nanovg.a
INCLUDES = bgfx-nanovg.h vs_nanovg_fill.bin.h fs_nanovg_fill.bin.h

include ./shader-embeded.mk

all: $(LIBS) $(INCLUDES)

libbgfx-nanovg.a: bgfx-nanovg.o
	$(AR) rcs $@ $<

bgfx-nanovg.o: bgfx-nanovg.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

install: all
	mkdir -p $(PREFIX)/{lib,include/bgfx-nanovg}
	cp $(LIBS) $(PREFIX)/lib
	cp $(INCLUDES) $(PREFIX)/include/bgfx-nanovg
