CC = gcc
LDLIBS = -lcurl -lcjson

build:
	$(CC) $(LDLIBS) -c -fPIC libpb.c -o libpb.o
	$(CC) $(LDLIBS) -shared libpb.o -o libpb.so

install: build
	sudo mv libpb.so /usr/lib/libpb.so
	sudo cp libpb.h /usr/local/include/libpb.h

clean:
	rm libpb.o
