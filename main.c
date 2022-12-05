/* The Mik webserver */
/* This is a work in progress small webserver */
/* Development started in december of 2022 by */
/* A. Coslovsky */

/* This program runs (as of now) exclusively on linux */
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 8080

int startup(u_short*);
void handle_request(int);
int get_request_line(int sock, char* buf, int size);

/* reads lines from requests  */
int get_request_line(int sock, char* buf, int size){
  int i = 0;
  char c = '\0';
  int n;

  /* while string hasnt ended and is not a newline */
  while((i < size - 1) && (c != '\n')){
    n = recv(sock, &c, 1, 0);
    /* if connection is not closed or didn't return an error, check below */
    if (n > 0){
      /* if it has successfully received the character from the socket */
      /* peek to see if it's a carriage feed and check if next character is a line return */
      if(c == '\r'){
        n = recv(sock, &c, 1, MSG_PEEK);
        if((n > 0) && (c == '\n')){
          recv(sock, &c ,1, 0);
        } else {
          c = '\n';
        }
      }
        buf[i] = c;
        i++;
    } else {
        c = '\n';
    }
  }
  buf[i] = '\0';

  return i;
}



/* Create new socket and bind it to port 80 */

int startup(u_short *port){
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  //initializing socket
  if(sock < 0){
    printf("error while creating socket \n");
    exit(1);
  }

  //bind socket
  if(bind(sock, (struct sockaddr_in*)&serv_addr, sizeof(serv_addr)) < 0){
    perror("bind");
    exit(1);
  }

  if(listen(sock, 5) < 0){
    perror("listen");
    exit(1);
  }
  return sock;
}

void handle_request(int client_sock){
 char buffer[2048];
 int numchars;
 char method[255];
 char url[255];
 char path[512];
 size_t i, j;
 struct stat* st;

 char *query_string = NULL;

 numchars = get_request_line(client_sock, buffer, sizeof(buffer));

 printf("%s\n", buffer);

 /* i=0; j=0; */
 /* while(!isspace((int)buffer[j]) && (i < sizeof(buffer) - 1)){ */
 /*   method[i] = buffer[j]; */
 /*   i++;j++; */
 /* } */
 /* method[i] = '\0'; */

}

int main(int argc, char *argv[]) {
  int server_sock;
  int client_sock;
  struct sockaddr_in client_name;
  int client_name_len = sizeof(client_name);

  server_sock = startup((u_short*)PORT);
  while(1){
    client_sock = accept(server_sock, (struct sockaddr*)&client_name, &client_name_len);
    if(client_sock == -1){
      perror("accept");
      exit(1);
    }
    handle_request(client_sock);
  }

  return 0;
}
