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

#include <stdbool.h>

#ifndef _LIBPB_H_
#define _LIBPB_H_

typedef void (*libpb_cb_t)(char *message);

int libpb_setup(char s_token[100]);
int libpb_post(char *text);
void libpb_register_cb(libpb_cb_t cb_ptr);
int libpb_get(void);
void *libpb_get_stream();
void libpb_cleanup(void);

#endif