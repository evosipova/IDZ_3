#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void request_room(int is_male, char* ip_address, int port) {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip_address);

  if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    perror("connect");
    exit(1);
  }

  char request[256];
  memset(request, 0, sizeof(request));
  sprintf(request, "%d", is_male);

  if (write(client_fd, request, strlen(request)) == -1) {
    perror("write");
    exit(1);
  }

  close(client_fd);
}

void request_updates(char* ip_address, int port) {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip_address);

  if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    perror("connect");
    exit(1);
  }

  char status[256];
  memset(status, 0, sizeof(status));

  if (read(client_fd, status, sizeof(status)) == -1) {
    perror("read");
    exit(1);
  }

  printf("%s", status);

  close(client_fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    exit(1);
  }

  char* ip_address = argv[2];
  int port = atoi(argv[3]);

  request_updates(ip_address, port);

  return 0;
}
