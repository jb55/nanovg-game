
LIBS=-lGLEW -lexample-common -lbgfx -lnanogui -lpthread \
     -lGL -lX11 -lglfw -lGLU -lnanovg -lbgfx-nanovg

LINKOPTS = $(LDFLAGS) $(LIBS)
CFLAGS = -g

all: hello nanovg

hello: hello.o
	$(CC) $(CFLAGS) $< $(LINKOPTS) -o $@

nanovg: nanovg.o
	$(CC) $(CFLAGS) $< $(LINKOPTS) -o $@

%.o: %.t
	terra $^ $(basename $@)

clean:
	rm -f hello hello.o
	rm -f nanovg nanovg.o
