
CFLAGS ?= -O2 -fPIC
PREFIX ?= /usr/local
LIBS = bgfx-nanovg.a
INCLUDES = bgfx-nanovg.h vs_nanovg_fill.bin.h fs_nanovg_fill.bin.h
LDFLAGS = -lnanovg -lbgfx $(shell pkg-config --libs bgfx)

include ./shader-embeded.mk

all: $(LIBS) $(INCLUDES)

bgfx-nanovg.a: bgfx-nanovg.o
	$(AR) cr $@ $<

bgfx-nanovg.o: bgfx-nanovg.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

install: all
	mkdir -p $(PREFIX)/{lib,include}
	mkdir -p $(PREFIX)/include/bgfx-nanovg
	cp $(LIBS) $(PREFIX)/lib
	cp $(INCLUDES) $(PREFIX)/include/bgfx-nanovg
