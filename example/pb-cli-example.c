// This file is part of libpb.

// libpb is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// libpb is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with libpb.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <libpb.h>

pthread_t stream_tid;
pthread_t user_input_tid;

bool cmp_message(char *test, char *match) {
	if (strcmp(test, match) == 0) {
		return true;
	} else {
		return false;
	}
}

libpb_cb_t recv_logic(char *message) {
	char last_message[100];
	FILE *history = fopen(".history", "r");
	fgets(last_message, 100, history);
	fclose(history);

	if (!cmp_message(last_message, message)) {
		printf("\r<--\t\t%s\n", message);
	}
	if (cmp_message(message, "testcb")) {
		printf("The callback works!\n");
	} 
}

void *user_input() {
	char user_string[100];
	fgets(user_string, 100, stdin);

	if (strcmp(user_string, "quit\n") == 0) {
		exit(0);
	} else if (strcmp(user_string, "\n") == 0) {
		printf("-->");
		user_input();
	} else {
		FILE *history = fopen(".history", "w");
		fprintf(history, "%s", user_string);
		fclose(history);

		libpb_post(user_string);
	}

	printf("-->");
	user_input();
}

int main(int argc, char *argv[]) {
	libpb_setup("");
	libpb_register_cb((libpb_cb_t) recv_logic);

	if (argc == 1) {
		printf("Please use a valid option.\nrun pb-cli -h to see available options\n");
		return 0;
	}

	int opt;
	while ((opt = getopt(argc,argv,"hvp:ist")) != -1) {
		switch (opt) {
			case 'h':
				printf("pb-cli v1.0\nCopyright 2018 Rumesh Sudhaharan\n");
				printf("Options:\n");
				printf("\t-h:\tPrint this help screen.\n");
				printf("\t-v:\tPrint version number.\n");
				printf("\t-p:\tsend message to your pushbullet account.\n");
				printf("\t-i:\tGet notified of incoming messages to your pushbullet account.\n");
				break;
			case 'v':
				printf("v1.0\n");
				break;
			case 'p':
				libpb_post(optarg);
				break;
			case 'i':
				printf("-->");
				libpb_register_cb((libpb_cb_t) recv_logic);

				pthread_create(&stream_tid, NULL, libpb_get_stream, NULL);
				pthread_create(&user_input_tid, NULL, user_input, NULL);

				pthread_join(stream_tid, NULL);
				pthread_join(user_input_tid, NULL);
				break;
			default:
				printf("Please use a valid option.\nrun pb-cli -h to see available options\n");
				break;
		}
	}

	libpb_cleanup();
	return 0;
}