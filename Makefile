ALLSRC  := $(wildcard src/*.c)
SRC     := $(filter-out src/test.c, $(ALLSRC))
TESTSRC := $(filter-out src/main.c, $(ALLSRC))
PKGS    := libevdev
CC      := clang
CFLAGS  := -Iheaders
CFLAGS  += -Wall -O2 -std=gnu17 $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs $(PKGS)) -lm
BINARY  := a.out

.PHONY: dev clean compile cnr

dev:
	bear -- make compile
	rm $(BINARY)

clean:
	$(RM) *.o $(BINARY)

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)

test-compile: $(TESTSRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)

cnr: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)
	./$(BINARY)
	$(RM) *.o $(BINARY)

tnr: $(TESTSRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINARY)
	./$(BINARY)
	$(RM) *.o $(BINARY)

