CFILES = examples/main_linux.c
HFILES = capi/*.h examples/gtk.h

CC = gcc
WARNINGS = -Wall -Wextra -Wpedantic -Wconversion -Wno-unused-parameter
SYSTEM_DEPS = `pkg-config --cflags gtk+-3.0 | sed "s>-I/>-isystem/>g"`
DBG_FLAGS = -g
CFLAGS = $(DBG_FLAGS) $(WARNINGS) $(SYSTEM_DEPS) -isystem ./cef_extracted -I.
LFLAGS = -L$(OUT) -Wl,-rpath=\$$ORIGIN `pkg-config --libs gtk+-3.0` -lX11 -lcef
OUT = ./Release
EXE = $(OUT)/cef_example
SELF = Makefile

all: $(EXE) $(SELF)

run: all
	$(EXE)

clean:
	rm -rf ./cef_extracted ./Release

$(SELF): extract_deps.mk
	touch $(SELF)

$(EXE): $(OUT)/libcef.so $(HFILES) $(CFILES) $(SELF)
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES) $(LFLAGS)

include extract_deps.mk
