
LIBS=-lGLEW -lexample-common -lbgfx -lnanogui -lpthread \
     -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg

LINKOPTS = $(LDFLAGS) $(LIBS)
CFLAGS = -g -std=c++11

all: hello nanovg

hello: hello.o
	$(CXX) $(CFLAGS) $< $(LINKOPTS) -o $@

nanovg: nanovg.o
	$(CXX) $(CFLAGS) $< $(LINKOPTS) -o $@

# %.o: %.t lang/nvg.t
# 	terra $^ $(basename $@)

%.o: %.cc Makefile
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f hello hello.o
	rm -f nanovg nanovg.o
