#include "./network.h"
#include "./base64.h"
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
 
#if !CURL_AT_LEAST_VERSION(7, 62, 0)
#error "this example requires curl 7.62.0 or later"
#endif


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

char *get_send_request(const char *host, size_t *len, char* image_path)
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

    // printf("requestFormatSize = %ld\n", requestFormatSize);
    // printf("fileSize = %ld\n", fileSize);
    // printf("requestEndSize = %ld\n", requestEndSize);
    // printf("requestSize = %d\n", requestSize);
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

gint get_image_from_url(gpointer data)
{
    char* input  = data;
    printf("Getting image from the url...");

    const char* prefix = "https://";
    const char* suffix = "\"}";

    const char* start = strstr(input, prefix);
    if (start == NULL) {
        printf("URL prefix not found.\n");
        return 1;
    }
    start += strlen(prefix);

    const char* end = strstr(start, suffix);
    if (end == NULL) {
        printf("URL suffix not found.\n");
        return 1;
    }

    // Calculate the length of host and path
    int hostLength = 0;
    int pathLength = 0;
    const char* pathStart = strstr(start, ".com/") + 5;
    if (pathStart != NULL) {
        hostLength = pathStart - start;
        pathLength = end - pathStart;
    }

    // Allocate memory for host and path
    char* host = (char*)malloc((hostLength + 1) * sizeof(char));
    char* path = (char*)malloc((pathLength + 1) * sizeof(char));

    // Copy host and path values
    strncpy(host, start, hostLength);
    host[hostLength-1] = '\0';
    strncpy(path, pathStart, pathLength);
    path[pathLength] = '\0';

    printf("Host: %s\n", host);
    printf("Path: %s\n", path);

    getchar();
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

    char* request = NULL;
    size_t request_len = asprintf(&request, 
                    "GET /%s HTTP/1.1\r\n"
                    "Connection: close\r\n"
                    "Host: %s\r\n\r\n", path, host);


    // Free dynamically allocated memory
    free(host);
    free(path);
    

    rewrite(ssl, request, request_len);


    ssize_t total_read = 0;
    printf("Now listening server response for update... \n");
    do {
        nread = SSL_read(ssl, buffer, BUFFER_SIZE-1);
        if (nread == -1)
            errx(1, "Read FAILED.");

        nwritten = write(STDOUT_FILENO, buffer, nread);
        total_read += nread;

        if (nwritten == -1)
            errx(1, "Write FAILED.");
        
    }
    while (nread != 0);

    return TRUE;
}

char *get_download_url (gpointer data)
{
    char *host = (char *) data;
    
    struct addrinfo *addr, *result;
    struct addrinfo hints;
    int sfd, s;

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

    
    char* request = "GET /url?key=epipaintor HTTP/1.1\r\n"
                    "Connection: close\r\n"
                    "Host: 4d3f2zejqh.execute-api.eu-west-1.amazonaws.com\r\n\r\n";

    size_t request_len = strlen(request);

    rewrite(ssl, request, request_len);

    printf("Now listening server response for update... \n");
    // Set the initial response buffer size
    size_t response_buffer_size = 512;

    // Allocate memory for the response buffer
    char* response_buffer = (char*)malloc(response_buffer_size);
    if (response_buffer == NULL) {
        perror("Failed to allocate memory for response buffer");
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(sfd);
        return FALSE;
    }

    // Receive and process the response in a loop until no bytes are received
    ssize_t total_bytes_received = 0;
    ssize_t bytes_received;
    while ((bytes_received = SSL_read(ssl, response_buffer + total_bytes_received, response_buffer_size - total_bytes_received - 1)) > 0) {
        total_bytes_received += bytes_received;

        // Check if the response buffer is full
        if (total_bytes_received >= (ssize_t) response_buffer_size - 1) {
            // Expand the response buffer size
            response_buffer_size *= 2;
            char* expanded_buffer = (char*)realloc(response_buffer, response_buffer_size);
            if (expanded_buffer == NULL) {
                perror("Failed to expand response buffer");
                free(response_buffer);
                SSL_shutdown(ssl);
                SSL_free(ssl);
                close(sfd);
                return FALSE;
            }
            response_buffer = expanded_buffer;
        }
    }

    // Check if there was an error in receiving
    if (bytes_received == -1) {
        perror("recv");
        free(response_buffer);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(sfd);
        return FALSE;
    }

    // Null-terminate the response buffer
    response_buffer[total_bytes_received] = '\0';

    printf("Presigned url = %s\n", response_buffer);
    return response_buffer;
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
    
    char* request = get_send_request(host, &request_len, image_path);
    // printf("request length is %lu\n", request_len);

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