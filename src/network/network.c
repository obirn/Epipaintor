// Imports for networking
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define BUFFER_SIZE 512

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include "./base64.c"

const size_t BUFFER_SIZE = 32;

void rewrite(int fd, const void *buf, size_t count)
{
    ssize_t written = 0, total_written = 0;
    const char *buffer_char = buf;

    do {
        written = write(fd, buffer_char + written, count);
        if (written == -1) errx(1, "Write FAILED.");
        total_written += written;
    }
    while (total_written < count);
}

char *encode_image(char* image_path)
{
    FILE *file;
    size_t decoded_len, encoded_len;
    unsigned char *decoded, *encoded;

    //Open file                                                                                                                                                                                                
    file = fopen(image_path , "rb");
    if (!file)
        errx(EXIT_FAILURE, "Unable to open file %s", image_path);

    //Get file length                                                                                                                                                                                          
    fseek(file, 0, SEEK_END);
    decoded_len=ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory                                                                                                                                                                                          
    decoded = (char *) malloc(decoded_len+1);
    if (!decoded)
    {
        fclose(file);
        errx(EXIT_FAILURE, "Not enough memory");
    }

    //Read file contents into buffer                                                                                                                                                                           
    fread(decoded, decoded_len, 1, file);
    int encodedLen = Base64encode_len(decoded_len);
    char *encoded = (char *) malloc(sizeof(char) * encodedLen);

    size_t tmp = Base64encode(encoded, decoded, decoded_len);
    printf("Pratical = %lu\n, Theoritical = %lu\n", tmp, encoded_len);

    printf("%s\n", encoded);
    fclose(file);
    free(decoded);

    return encoded;
}

char *build_query(const char *host, size_t *len, char* image_path)
{
    char *encoded_image = encode_image(image_path);

    char *request = NULL;

    ssize_t request_len = 
        asprintf(&request, "GET http://www.%s/?img=%s HTTP/1.0\r\n\r\n", host, encoded_image);
    
    if (request_len == -1) errx(1, "Asprint FAILED.");

    return request;
}

void print_page(const char *host)
{
    char buffer[BUFFER_SIZE];
    
    struct addrinfo *addr, *result;
    struct addrinfo hints;
    int sfd, s;
    ssize_t nread, nwritten;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(host, "80", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (addr = result; addr != NULL; addr = addr->ai_next) {
        sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, addr->ai_addr, addr->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    freeaddrinfo(result);

    if (addr == NULL) {
        errx(1, "Couldn't connect.");
    }

    size_t request_len;
    char* request;
    request = build_query(host, &request_len);

    rewrite(sfd, request, request_len);

    free(request);

    do {
        nread = read(sfd, buffer, BUFFER_SIZE);
        if (nread == -1)
            errx(1, "Read FAILED.");

        nwritten = write(STDOUT_FILENO, buffer, nread);
        if (nwritten == -1)
            errx(1, "Write FAILED.");

    }
    while (nread != 0);

    close(sfd);
}