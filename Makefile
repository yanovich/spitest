CC=arm-linux-gnueabihf-gcc
PROGRAM=spitest

.PHONY: all
all: $(PROGRAM)

%.o: %.c *.h Makefile
	$(CC) -c $< -o $@

$(PROGRAM): $(PROGRAM).o
	$(CC) -o $@ $^

clean:
	rm *.o $(PROGRAM)

test: all
	./$(PROGRAM)
