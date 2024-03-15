#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "helpers.h"
#include "requests.h"
#include "cJSON.h"


int main(int argc, char *argv[]) 
{
    char *message;
    char *response;
    int sockfd;
    char input[256];
    char usr_input[256];

    char cookie[256] = "";
    char username[256];
    char password[256];
    char* JWT = NULL;
// variabila json
    cJSON* json = NULL;

    while(1) 
    {
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", usr_input);
        response = NULL;
        if (strcmp(usr_input, "exit") == 0)
        break;

        if(strcmp(usr_input, "exit") != 0) 
        {
			// deschid conexiunea
            
			// comanda register 
            if(strcmp(usr_input, "register") == 0) 
            {
				// citesc numele si parola
                printf("username=");
                fgets(username, sizeof(username), stdin);
                char *username2 = malloc(100);
                memcpy(username2, username, strlen(username) - 1);
                printf("password=");
                fgets(password, sizeof(password), stdin);
                char *passwd2 = malloc(100);
                memcpy(passwd2, password, strlen(password) - 1);
                cJSON* json_register = cJSON_CreateObject();
                cJSON_AddStringToObject(json_register, "username", username2);
                cJSON_AddStringToObject(json_register, "password", passwd2);
                char* body = cJSON_PrintUnformatted(json_register);
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register", "application/json", &body, 1, NULL, 0, JWT);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                char *token = strtok(response, " ");
                token = strtok(NULL, " ");
                if (strcmp(token, "201") == 0) {
					printf("Contul a fost creat cu succes\n");
				} else {
                    printf("eroare : Acest nume: %s este ocupat!\n", username);
                } 
                cJSON_Delete(json_register);
                close_connection(sockfd);
            }
            else if (strcmp(usr_input, "login") == 0) {
                printf("username=");
                fgets(username, sizeof(username), stdin);
                char *username2 = malloc(100);
                memcpy(username2, username, strlen(username) - 1);
                printf("password=");
                fgets(password, sizeof(password), stdin);
                char *passwd2 = malloc(100);
                memcpy(passwd2, password, strlen(password) - 1);
                cJSON* json_login = cJSON_CreateObject();
                cJSON_AddStringToObject(json_login, "username", username2);
                cJSON_AddStringToObject(json_login, "password", passwd2);
                char* body = cJSON_PrintUnformatted(json_login);
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login", "application/json", &body, 1, NULL, 0, JWT);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                close_connection(sockfd);
                //printf("%s\n", response);
                if(strstr(response, "Set-Cookie") == NULL) {
                    printf("eroare : Nu exista cont cu asa nume sau parola\n");
                }
                else {
                    char* cookie_start = strstr(response, "Cookie:");
                    char* cookie_end = strchr(cookie_start, ';');
                    strncpy(cookie, cookie_start + 8, cookie_end - (cookie_start + 8));
                    cookie[cookie_end - (cookie_start + 8)] = '\0';
                    if(JWT != NULL) {
                        free(JWT);
                        JWT = NULL;
                    }
                    printf("Esti logat\n");
                }
                cJSON_Delete(json_login);
                
            }
            else if (strcmp(usr_input, "enter_library") == 0) {
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0) {
                    printf("Conectare esuata\n");
                    exit(1);
                }
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.254.242.81", "/api/v1/tema/library/access", NULL, cookie, 1, JWT);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                if (strstr(response, "token") == NULL) {
                    printf("eroare : Nu esti autentificat\n");
                } else {
                    char* token_start = strstr(response, "token");
                    token_start += 8;
                    char* token_end = strchr(token_start, '\"');
                    *token_end = '\0';
                    if (JWT != NULL) {
                        free(JWT);
                    }
                    JWT = strdup(token_start);
                    printf("Ai primit acces la biblioteca\n");
                    printf("Tokenul tau JWT este:\n%s\n", JWT);
                }
                cJSON_Delete(json);
                close_connection(sockfd);
            }

            else if (strcmp(usr_input, "get_books") == 0) {
                if(JWT != NULL) {
                    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                    message = compute_get_request("34.254.242.81", "/api/v1/tema/library/books", NULL, cookie, 1, JWT);
                    send_to_server(sockfd, message);
                    response = receive_from_server(sockfd);
                    close_connection(sockfd);
                    char *booksList = strchr(response, '[');
                    printf("%s\n", booksList);

                } else {
                    printf("eroare : Nu ai  acces la  biblioteca\n");
                }
                
            }
            else if (strcmp(usr_input, "get_book") == 0) {
                if (JWT != NULL) {
                    char *id_book = malloc(20);
                    printf("id=");
                    fgets(id_book, 20, stdin);
                    char *id = calloc(20, 1);
                    memcpy(id, id_book, strlen(id_book) - 1);
                    char addr[100] = "/api/v1/tema/library/books/";
                    strcat(addr, id);
                    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                    message = compute_get_request("34.254.242.81", addr , NULL, cookie, 1, JWT);
                    send_to_server(sockfd, message);
                    response = receive_from_server(sockfd);
                    close_connection(sockfd);

                    if(strstr(response, "error") != NULL) {
                        printf("eroare : Cartea nu a fost gasita\n");
                    } else {
                        printf("%s\n", strchr(response, '{'));
                    }
                } else {
                    printf("eroare : Nu ai  acces la  biblioteca\n");
                }
            }
            else if (strcmp(usr_input, "add_book") == 0) {
                if (JWT != NULL) {
                    char data[256];
                    printf("title=");
                    fgets(data, sizeof(data), stdin);
                    cJSON* json_add = cJSON_CreateObject();
                    data[strlen(data) -1 ] = '\0';
                    cJSON_AddStringToObject(json_add, "title", data);
                    printf("author=");
                    fgets(data, sizeof(data), stdin);
                    data[strlen(data) - 1] = '\0';
                    cJSON_AddStringToObject(json_add, "author", data);
                    printf("genre=");
                    fgets(data, sizeof(data), stdin);
                    data[strlen(data) - 1] = '\0';
                    cJSON_AddStringToObject(json_add, "genre", data);
                    printf("publisher=");
                    fgets(data, sizeof(data), stdin);
                    data[strlen(data) - 1] = '\0';
                    cJSON_AddStringToObject(json_add, "publisher", data);
                    printf("page_count=");
                    fgets(data, sizeof(data), stdin);
                    data[strlen(data) - 1] = '\0';
                    cJSON_AddNumberToObject(json_add, "page_count", atoi(data));
                    char* body = cJSON_PrintUnformatted(json_add);
                    sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                    message = compute_post_request("34.254.242.81", "/api/v1/tema/library/books", "application/json", &body, 1, NULL, 0, JWT);
                    send_to_server(sockfd, message);
                    response = receive_from_server(sockfd);
                    printf("%s\n", response);
                    cJSON_Delete(json_add);
                    close_connection(sockfd);
                }
            }
            else if (strcmp(usr_input, "delete_book") == 0) {
                char *id_book = malloc(20);
                printf("id=");
                fgets(id_book, 20, stdin);
                char *id = calloc(20, 1);
                memcpy(id, id_book, strlen(id_book) - 1);
                char addr[100] = "/api/v1/tema/library/books/";
                strcat(addr, id);
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_delete_request("34.254.242.81", addr, NULL, cookie, 1, JWT);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                printf("%s\n", response);
                close_connection(sockfd);
            }
            else if (strcmp(usr_input, "logout") == 0) {
                JWT = NULL;
                sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.254.242.81", "/api/v1/tema/auth/logout", NULL, cookie, 1, JWT);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                printf("Logout cu succes\n");
                close_connection(sockfd);
            }
            else if (strcmp(usr_input, "exit") == 0) {
                JWT = NULL;
                cookie[0] = '\0';
                break;
            }
        }
    }
    return 0;
}
