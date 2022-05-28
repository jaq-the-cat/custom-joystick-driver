SRC     := $(wildcard src/*.c)
PKGS    := libevdev
CC      := clang
CFLAGS  := -Iheaders
CFLAGS  += -Wall -O2 -std=gnu17 $(shell pkg-config --cflags $(PKGS))
LDFLAGS := -lxdo
LDFLAGS += $(shell pkg-config --libs $(PKGS))

.PHONY: dev clean compile cnr

dev:
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
	echo $(LDFLAGS) | tr " " "\n" >> compile_flags.txt

clean:
	$(RM) *.o a.out

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

cnr: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out
	./a.out
	$(RM) *.o a.out
