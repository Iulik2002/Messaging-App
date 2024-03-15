#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

#define BUFLEN 4096
#define LINELEN 1000

char* compute_get_request(char* host, char* url, char* query_params,
                          char* cookie, int cookies_count, char* JWT)
{
    char* message = malloc(BUFLEN);
    char* line = malloc(LINELEN);

    if (query_params != NULL) {
        snprintf(line, LINELEN, "GET %s/%s HTTP/1.1", url, query_params);
    } else {
        snprintf(line, LINELEN, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    snprintf(line, LINELEN, "Host: %s", host);
    compute_message(message, line);

    if (cookie != NULL) {
        int i;
        for (i = 0; i < cookies_count; i++) {
            snprintf(line, LINELEN, "Cookie: %s", cookie);
            compute_message(message, line);
        }
    }

    if (JWT != NULL) {
        snprintf(line, LINELEN, "Authorization: Bearer %s\r\n", JWT);
        strcat(message, line);
    }

    strcat(message, "\r\n");
    free(line);
    return message;
}

char* compute_delete_request(char* host, char* url, char* query_params,
                             char* cookie, int cookies_count, char* JWT)
{
    char* message = malloc(BUFLEN);
    char* line = malloc(LINELEN);

    if (query_params != NULL) {
        snprintf(line, LINELEN, "DELETE %s/%s HTTP/1.1", url, query_params);
    } else {
        snprintf(line, LINELEN, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    snprintf(line, LINELEN, "Host: %s", host);
    compute_message(message, line);

    if (cookie != NULL) {
        int i;
        for (i = 0; i < cookies_count; i++) {
            snprintf(line, LINELEN, "Cookie: %s", cookie);
            compute_message(message, line);
        }
    }

    if (JWT != NULL) {
        snprintf(line, LINELEN, "Authorization: Bearer %s\r\n", JWT);
        strcat(message, line);
    }

    strcat(message, "\r\n");
    free(line);
    return message;
}

char* compute_post_request(char* host, char* url, char* content_type, char** body_data,
                           int body_data_fields_count, char** cookies, int cookies_count, char* JWT)
{
    char* message = malloc(BUFLEN);
    char* line = malloc(LINELEN);
    char* body_data_buffer = malloc(LINELEN);

    snprintf(line, LINELEN, "POST %s HTTP/1.1\r\n", url);
    strcpy(message, line);

    snprintf(line, LINELEN, "Host: %s", host);
    compute_message(message, line);

    int msg_size = 0;
    if (body_data) {
        size_t i;
        for (i = 0; i < body_data_fields_count - 1; ++i) {
            compute_message(body_data_buffer, body_data[i]);
            compute_message(body_data_buffer, "&");

            msg_size += strlen(body_data[i]) + 1;
        }

        compute_message(body_data_buffer, body_data[i]);
        msg_size += strlen(body_data[i]);
    }

    snprintf(line, LINELEN, "Content-Length: %d", msg_size);
    compute_message(message, line);

    snprintf(line, LINELEN, "Content-Type: %s", content_type);
    compute_message(message, line);

    if (cookies != NULL) {
        int i;
        for (i = 0; i < cookies_count; i++) {
            snprintf(line, LINELEN, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }

    if (JWT != NULL) {
        snprintf(line, LINELEN, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    strcat(message, "\r\n");
    strcat(message, body_data_buffer);
    free(line);
    return message;
}
