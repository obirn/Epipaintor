#include "./network.h"
#include "./base64.h"
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <gtk/gtk.h>


void rewrite(SSL* ssl, const void *buf, size_t count)
{
    ssize_t written = 0, total_written = 0;
    const char *buffer_char = buf;

    do {
        // write(STDOUT_FILENO, buffer_char + written, count);
        written = SSL_write(ssl, buffer_char + written, count);
        if (written == -1) errx(1, "Write FAILED.");
        total_written += written;
        // printf("%ld/%ld\n", total_written, count);
    }
    while (total_written < (ssize_t) count);
}

char *send_query(const char *host, size_t *len, char* image_path)
{
    // size_t encodedLen;
    (void) host;
    // char *encoded_image = encode_image(image_path, &encodedLen);
    // (void) encoded_image;

    const char* requestFormat = "POST /send HTTP/1.1\r\n"
                                "Host: 4d3f2zejqh.execute-api.eu-west-1.amazonaws.com\r\n"
                                "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryABC123\r\n"
                                "Connection: close\r\n"
                                "Content-Length: %d\r\n\r\n"
                                "------WebKitFormBoundaryABC123\r\n"
                                "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n"
                                "Content-Type: image/jpeg\r\n\r\n";

    const char* requestEnd = "\r\n------WebKitFormBoundaryABC123--\r\n";

    // Open the image file
    FILE* file = fopen(image_path, "rb");
    if (!file) {
        perror("fopen");
    }

    // Calculate the content length
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    long requestFormatSize = snprintf(NULL, 0, requestFormat, fileSize);
    long requestEndSize = strlen(requestEnd);
    int requestSize = requestFormatSize + fileSize + requestEndSize;

    printf("requestFormatSize = %ld\n", requestFormatSize);
    printf("fileSize = %ld\n", fileSize);
    printf("requestEndSize = %ld\n", requestEndSize);
    printf("requestSize = %d\n", requestSize);
    // getchar();

    char* request = malloc(requestSize + 1);
    if (!request) {
        fprintf(stderr, "Failed to allocate memory for the request.\n");
        fclose(file);
    }

    // Format the request
    snprintf(request, requestSize, requestFormat, fileSize);
    char* requestBody = request + requestFormatSize;
    fread(requestBody, 1, fileSize, file);
    printf("(request + requestSize) - (requestBody + fileSize) = %ld\n", (request + requestSize) - (requestBody + fileSize));
    printf("requestEnd = %ld\n", requestEndSize);

    strcpy(requestBody + fileSize, requestEnd);

    // printf("content length = %ld\n", fileSize);
    // printf("requestFormat = %s\n", request);
    // printf("requestEnd = %s\n", requestBody + encodedLen);
    // printf("request size = %d\n", requestSize);
    // getchar();
    *len = requestSize;
    return request;
}

gint update_image (gpointer data)
{
    char *host = (char *) data;
    char buffer[BUFFER_SIZE];
    
    struct addrinfo *addr, *result;
    struct addrinfo hints;
    int sfd, s;
    ssize_t nread, nwritten;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(host, "443", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (addr = result; addr != NULL; addr = addr->ai_next) {
        sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        // printf("socket = %i\n", sfd);
        if (sfd == -1)
            continue;

        if (connect(sfd, addr->ai_addr, addr->ai_addrlen) == 0)
            break;

        // printf("Connect: %s\n", strerror(errno));

        close(sfd);
    }

    freeaddrinfo(result);

    if (addr == NULL) {
        errx(1, "Couldn't connect.");
    }

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
        errx(EXIT_FAILURE, "Failed to create SSL context.\n");

    // Establish the SSL/TLS connection
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "Failed to create SSL object.\n");
    }

    if (SSL_set_fd(ssl, sfd) == 0) {
        fprintf(stderr, "Failed to set SSL file descriptor.\n");
    }

    if (SSL_connect(ssl) != 1) {
        fprintf(stderr, "Failed to establish SSL connection.\n");
        ERR_print_errors_fp(stderr);
    }

    
    char* request = "POST /update HTTP/1.1\r\n"
                    "Connection: close\r\n"
                    "Host: 4d3f2zejqh.execute-api.eu-west-1.amazonaws.com\r\n\r\n";
    size_t request_len = strlen(request);

    rewrite(ssl, request, request_len);

    FILE* file = fopen("../cache/img_buff.bmp", "w+");

    printf("Now listening server response for update... \n");
    do {
        nread = SSL_read(ssl, buffer, BUFFER_SIZE-1);
        printf("nread = %ld\n", nread);
        buffer[BUFFER_SIZE-1] = 0;
        if (nread == -1)
            errx(1, "Read FAILED.");

        nwritten = write(STDOUT_FILENO, buffer, nread);
        if (nwritten == -1)
            errx(1, "Write FAILED.");

        nwritten = fputs(buffer, file);
        printf("wrote in file = %ld\n", nwritten);
        if (nwritten == -1)
            errx(1, "Write FAILED.");
            
    }
    while (nread != 0);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sfd);
    fclose(file);

    // returns true so that the function get called back.
    return TRUE;
}

void send_image(const char *host, char* image_path)
{
	char buffer[BUFFER_SIZE];
    
    struct addrinfo *addr, *result;
    struct addrinfo hints;
    int sfd, s;
    ssize_t nread, nwritten;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(host, "443", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (addr = result; addr != NULL; addr = addr->ai_next) {
        sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        // printf("socket = %i\n", sfd);
        if (sfd == -1)
            continue;

        if (connect(sfd, addr->ai_addr, addr->ai_addrlen) == 0)
            break;

        // printf("Connect: %s\n", strerror(errno));

        close(sfd);
    }

    freeaddrinfo(result);

    if (addr == NULL) {
        errx(1, "Couldn't connect.");
    }

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
        errx(EXIT_FAILURE, "Failed to create SSL context.\n");

    // Establish the SSL/TLS connection
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "Failed to create SSL object.\n");
    }

    if (SSL_set_fd(ssl, sfd) == 0) {
        fprintf(stderr, "Failed to set SSL file descriptor.\n");
    }

    if (SSL_connect(ssl) != 1) {
        fprintf(stderr, "Failed to establish SSL connection.\n");
        ERR_print_errors_fp(stderr);
    }

    size_t request_len;
    
    char* request = send_query(host, &request_len, image_path);
    printf("request length is %lu\n", request_len);

    rewrite(ssl, request, request_len);

    free(request);

    printf("Now listening server response after sending... \n");
    do {
        nread = SSL_read(ssl, buffer, BUFFER_SIZE);
        if (nread == -1)
            errx(1, "Read FAILED.");

        nwritten = write(STDOUT_FILENO, buffer, nread);
        if (nwritten == -1)
            errx(1, "Write FAILED.");
    }
    while (nread != 0);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sfd);
}