
LIBS=-lGLEW -lexample-common -lbgfx -lpthread -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg -lbgfx-imgui -lnanogui

LINKOPTS = $(LDFLAGS) $(LIBS)
TFLAGS = -g
CFLAGS = -g -std=c++11 -Wno-unused-variable -O0

OBJS = src/nanovg.o src/nanosvg-test.o

TOBJS = src/nanovg-terra

all: nanovg-terra

hello: src/hello.o
	$(CXX) $(CFLAGS) $< $(LINKOPTS) -o $@

nanovg: $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LINKOPTS) -o $@

# %.o: %.t lang/nvg.t
# 	terra $^ $(basename $@)

# src/nanovg-terra.o: src/nanovg.t
# 	cd src && terra -g nanovg.t
# 	mv src/nanovg.o src/nanovg-terra.o

nanovg-terra: src/nanovg.to
	$(CXX) $(CFLAGS) $< $(LINKOPTS) -o $@

%.to: %.t src/lang/nvg.t
	cd src && terra $(TFLAGS) $(subst src/,,$<)

%.o: %.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f hello hello.o
	rm -f src/*.o
	rm -f src/*.to
	rm -f nanovg{,-terra}
	rm -f $(OBJS)
