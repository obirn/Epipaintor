#ifndef NETWORK_H
#define NETWORK_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define BUFFER_SIZE 512

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

void rewrite(int fd, const void *buf, size_t count);
char *encode_image(char* image_path);
char *build_query(const char *host, size_t *len, char* image_path);
void send_image(const char *host, char* image_path);

#endif 