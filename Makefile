
LIBS=-lGLEW -lexample-common -lbgfx -lpthread -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg -lbgfx-imgui -lnanogui

LINKOPTS = $(LDFLAGS) $(LIBS)
TFLAGS = -g
CXXFLAGS = -g -std=c++11 -Wno-unused-variable -O0
CFLAGS = -g -Wno-unused-variable -O0

OBJS = src/nanovg.o \
       src/nanosvg.o \
       src/game.o \
       src/logging.o \
       src/entity.o

TOBJS = src/nanovg.to

all: nanovg

nanovg: $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LINKOPTS) -o $@

TAGS:
	etags -o - src/*.cc > $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f hello hello.o
	rm -f src/*.o
	rm -f src/*.to
	rm -f nanovg{,-terra}
	rm -f $(OBJS)

# terra stuff

nanovg-terra: $(OBJS) $(TOBJS)
	$(CXX) $(CFLAGS) $^ $(LINKOPTS) -o $@

%.to: %.t src/lang/nvg.t
	cd src && terra $(TFLAGS) $(subst src/,,$<)

# ^ terra stuff

.PHONY: TAGS
