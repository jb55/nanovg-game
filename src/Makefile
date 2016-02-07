
LIBS=-lGLEW -lexample-common -lbgfx -lpthread \
     -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg \
     -lbgfx-imgui -lnanogui

LINKOPTS = $(LDFLAGS) $(LIBS)
CFLAGS = -g -std=c++11 -Wno-unused-variable

OBJS = nanovg.o nanosvg-test.o

all: nanovg

hello: hello.o
	$(CXX) $(CFLAGS) $< $(LINKOPTS) -o $@

nanovg: $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LINKOPTS) -o $@

# %.o: %.t lang/nvg.t
# 	terra $^ $(basename $@)

%.o: %.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f hello hello.o
	rm -f nanovg
	rm -f $(OBJS)
