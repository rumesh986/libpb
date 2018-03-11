CC = gcc
LDLIBS = -lpthread -lcurl -lcjson -lpb

build:
	$(CC) -c pb-cli-example.c -o pb-cli-example.o
	$(CC) pb-cli-example.o $(LDLIBS) -o pb-cli-example

clean: build
	rm pb-cli-example.o