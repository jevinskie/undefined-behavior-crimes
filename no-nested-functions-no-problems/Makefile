TARGETS := blocks-not-nested-functions

CFLAGS := -Wall -Wextra -Wpedantic -Wno-gnu-statement-expression -g -Ofast -flto=full --std=c17 $(CFLAGS)

all: $(TARGETS)
.PHONY: clean

clean:
	rm -rf $(TARGETS) $(foreach TARGET,$(TARGETS),$(TARGET).dSYM)

blocks-not-nested-functions: blocks-not-nested-functions.c
	$(CC) -o $@ $^ $(CFLAGS)
