CC       = gcc
CFLAGS   = -O2 -Wall -Wextra
OMPFLAGS = -fopenmp
RAYLIB_DIR =
UNAME := $(shell uname -s 2>/dev/null)

ifeq ($(OS),Windows_NT)
    RAYLIB_LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(UNAME),Darwin)
    BREW := $(shell brew --prefix 2>/dev/null)
    OMPFLAGS = -Xpreprocessor -fopenmp -I$(BREW)/opt/libomp/include -L$(BREW)/opt/libomp/lib -lomp
    RAYLIB_LIBS = -I$(BREW)/include -L$(BREW)/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
else
    RAYLIB_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifneq ($(RAYLIB_DIR),)
    RAYLIB_LIBS := -I$(RAYLIB_DIR)/include -L$(RAYLIB_DIR)/lib $(RAYLIB_LIBS)
endif

all: serial parallel ensemble test

serial: serial_traffic
parallel: parallel_traffic
ensemble: ensemble_traffic
test: test_traffic

serial_traffic: serial_traffic.c traffic.c traffic.h
	$(CC) $(CFLAGS) -Wno-unknown-pragmas -o $@ serial_traffic.c traffic.c

parallel_traffic: parallel_traffic.c traffic.c traffic.h
	$(CC) $(CFLAGS) -o $@ parallel_traffic.c traffic.c $(OMPFLAGS)

ensemble_traffic: ensemble_traffic.c traffic.c traffic.h
	$(CC) $(CFLAGS) -o $@ ensemble_traffic.c traffic.c $(OMPFLAGS)

test_traffic: test_traffic.c traffic.c traffic.h
	$(CC) $(CFLAGS) -o $@ test_traffic.c traffic.c $(OMPFLAGS)

ifeq ($(RENDER_OMP),1)
    RENDER_FLAGS = $(OMPFLAGS)
else
    RENDER_FLAGS = -Wno-unknown-pragmas
endif

renderer: renderer.c traffic.c traffic.h
	$(CC) $(CFLAGS) -Iinclude -o $@ renderer.c traffic.c $(RENDER_FLAGS) $(RAYLIB_LIBS) -lm

check: test_traffic
	./test_traffic

bench: parallel_traffic ensemble_traffic
	./bench.sh

clean:
	rm -f serial_traffic parallel_traffic ensemble_traffic test_traffic renderer *.exe *.o

.PHONY: all serial parallel ensemble test renderer check bench clean
