UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
INCFLAGS  = -I vecmath/include
LINKFLAGS = -L. -lRK4

FRAMEWORKS = -framework GLUT
FRAMEWORKS += -framework OpenGL

else
INCFLAGS  = -I vecmath/include
INCFLAGS += -I /usr/include/GL
LINKFLAGS = -L. -lRK4 -lglut -lGL -lGLU
endif
CFLAGS    = -g -Wall -ansi
CC        = g++
SRCS      = $(wildcard *.cpp)
SRCS     += $(wildcard vecmath/src/*.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a3

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(FRAMEWORKS) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)
