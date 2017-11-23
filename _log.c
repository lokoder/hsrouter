#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "_log.h"

void d(const char *message) {

    fprintf(stderr, "### %s\n", message);
}

void err(void) {

    fprintf(stderr, "%s\n", strerror(errno));
}

