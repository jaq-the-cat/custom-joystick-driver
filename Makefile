SRC     := $(wildcard src/*.c)
PKGS    := libevdev
CC      := clang
CFLAGS  := -Iheaders
CFLAGS  += -Wall -O2 -std=gnu17 $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs $(PKGS))
BINARY := a.out

.PHONY: dev clean compile cnr

dev:
	bear -- make compile
	rm $(BINARY)

clean:
	$(RM) *.o $(BINARY)

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)

cnr: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)
	./$(BINARY)
	$(RM) *.o $(BINARY)
