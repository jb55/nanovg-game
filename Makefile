
LIBS=-lGLEW -lexample-common -lbgfx -lpthread -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg -lbgfx-imgui -lnanogui -lchipmunk

LINKOPTS = $(LDFLAGS) $(LIBS)
TFLAGS = -g
CXXFLAGS = -g -std=c++11 -Wno-unused-variable -O0

OBJS = src/nanovg.o \
       src/nanosvg.o \
       src/game.o \
       src/logging.o \
       src/globals.o \
       src/world.o \
       src/entity.o

TOBJS = src/nanovg.to

all: nanovg

include $(OBJS:.o=.d)

nanovg: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LINKOPTS) -o $@

TAGS:
	etags -o - src/*.cc > $@

clean:
	rm -f hello hello.o
	rm -f src/*.o
	rm -f src/*.to
	rm -f src/*.d*
	rm -f nanovg{,-terra}
	rm -f $(OBJS)

%.d: %.cc Makefile
	@rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,src/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY: TAGS

