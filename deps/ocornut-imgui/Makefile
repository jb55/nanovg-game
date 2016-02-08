
CFLAGS = -O2 -std=c+11

OBJS = imgui_draw.o imgui_demo.o imgui_wm.o

all: libbgfx-imgui.a

libbgfx-imgui.a: $(OBJS)
	ar rcs libbgfx-imgui.a $(OBJS)

*.o: *.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
