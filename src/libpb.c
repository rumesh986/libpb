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
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "libpb.h"

char header_string[49];
char url_string[74];
char token[100];

bool cb_registered = false;
libpb_cb_t registered_cb;



int libpb_setup(char s_token[100]) {
	strcpy(token, s_token);

	strcpy(header_string, "Access-Token: ");
	strcat(header_string, token);

	strcpy(url_string, "https://stream.pushbullet.com/streaming/");
	strcat(url_string, token);

	curl_global_init(CURL_GLOBAL_ALL);
}

size_t post_cb(char *data, size_t size, size_t nmemb) {
	return size * nmemb;
}

int libpb_post(char *text) {
	char *post_data;
	char *post_data_prefix = "type=note&body=";
	size_t length = strlen(text) + strlen(post_data_prefix);

	post_data = malloc(length);
	strcpy(post_data, post_data_prefix);
	strcat(post_data, text);

	CURL *post_handle = curl_easy_init();

	struct curl_slist *access_headers = NULL;
	access_headers = curl_slist_append(access_headers, header_string);

	curl_easy_setopt(post_handle,CURLOPT_HTTPHEADER, access_headers);
	curl_easy_setopt(post_handle, CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(post_handle, CURLOPT_URL, "https://api.pushbullet.com/v2/pushes");
	curl_easy_setopt(post_handle, CURLOPT_POST, 1);
	curl_easy_setopt(post_handle, CURLOPT_WRITEFUNCTION, post_cb);

	curl_easy_perform(post_handle);
	curl_easy_cleanup(post_handle);
	free(post_data);

	return 0;
}

void libpb_register_cb(libpb_cb_t cb_ptr) {
	cb_registered = true;
	registered_cb = cb_ptr;
}

size_t get_cb(char *data, size_t size, size_t nmemb) {
	cJSON *jsonstring = cJSON_Parse(data);
	const cJSON *body = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(jsonstring, "pushes")->child, "body");

	if (cb_registered) {
		(*registered_cb)(body->valuestring);
	} else {
		printf("\r<--\t\t%s\n", body->valuestring);
	}

	return size * nmemb;
}

int libpb_get(void) {
	CURL *get_handle = curl_easy_init();
	struct curl_slist *access_headers = NULL;
	access_headers = curl_slist_append(access_headers, header_string);

	curl_easy_setopt(get_handle,CURLOPT_HTTPHEADER, access_headers);
	curl_easy_setopt(get_handle, CURLOPT_URL, "https://api.pushbullet.com/v2/pushes?limit=1");
	curl_easy_setopt(get_handle, CURLOPT_WRITEFUNCTION, get_cb);

	curl_easy_perform(get_handle);
	curl_easy_cleanup(get_handle);

	return 0;
}

size_t get_stream_cb(char *data, size_t size, size_t nmemb) {
	cJSON *jsonstring = cJSON_Parse(data);
	char *type_value = jsonstring->child->valuestring;

	if (strcmp(type_value, "tickle") == 0) {
		char *subtype_value = jsonstring->child->next->valuestring;

		if (strcmp(subtype_value, "push") == 0) {
			libpb_get();
		}
	}
	return size * nmemb;
}

void *libpb_get_stream() {
	CURL *get_stream_handle = curl_easy_init();

	curl_easy_setopt(get_stream_handle, CURLOPT_URL, url_string);
	curl_easy_setopt(get_stream_handle, CURLOPT_WRITEFUNCTION, get_stream_cb);

	curl_easy_perform(get_stream_handle);
	curl_easy_cleanup(get_stream_handle);
}

void libpb_cleanup(void) {
	curl_global_cleanup();
}