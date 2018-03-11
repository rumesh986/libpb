CC = gcc
LDLIBS = -lcurl -lcjson

build:
	$(CC) -c -fPIC libpb.c -o libpb.o
	$(CC) -shared libpb.o -o libpb.so

install: build
	sudo mv libpb.so /usr/lib/libpb.so
	sudo cp libpb.h /usr/local/include/libpb.h

clean: build
	rm libpb.o
