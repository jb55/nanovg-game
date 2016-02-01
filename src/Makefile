
# LIBS=-Wl,-Bstatic -lGLEW -lbgfx -lexample-common \
# 	 	 -Wl,-Bdynamic -lpthread -lGL -lX11 -lglfw -lGLU

LIBS=-lGLEW -lexample-common -lbgfx -lnanogui -lpthread \
     -lGL -lX11 -lglfw -lGLU -lbgfx-nanovg

LINKOPTS = $(LDFLAGS) $(LIBS)
CFLAGS = -g

all: hello

hello: hello.o
	$(CC) $(CFLAGS) $(LINKOPTS) $<  -o $@ 

%.o: %.t
	terra $^ $(basename $@)

clean:
	rm -f hello hello.o
