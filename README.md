# libpb
Library to use the pushbullet in a C program

## Installation
Go to the `src` directory and run `make install` to install the library.
The library is installed to `/usr/lib/` and the header is placed in `/usr/local/include/`. An example program is included in the `example` folder. The example program can be built with `make build`.
This library requires curl and cjson to work.

## Usage
Include libpb in your program with `#include <libpb.h>` and add the `-lpb` flag to gcc when compiling.

#### libpb_setup(char s_token[100]);
In order to use the pushbullet services, you need to run `libpb_setup();` with your access token as the parameter. run `libpb_cleanup();` once you are done with the service.

#### libpb_post(char \*text);
This function will send `text` to your account.

#### libpb_register_cb(libpb_cb_t cb_ptr);
This function allows you to add your own callback function for dealing with recieved messages. Your function should be of the type `libpb_cb_t` which is basically `void function(char *message);` where message is the recieved message.
This will be called when libpb_get() is run.

#### libpb_get();
This function gets the last message in your account and prints it. you can register your own function to deal with the recieved message if you like with `libpb_register_cb();`. If you register a callback function, the last message will only be sent to your callback, it will no longer be printed.

#### libpb_get_stream();
This function prints out the all messages recieved while it is running. It is better to run this on a seperate thread like in the example program. you can register your own function to deal with the recieved message if you like with `libpb_register_cb();`.

#### libpb_cleanup();
Run this function once you are finished with using pushbullet.