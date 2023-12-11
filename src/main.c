#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int s = socket(AF_INET, SOCK_STREAM, 0);

  if (s == -1) {
    perror("socket error");
    return 1;
  }

  struct sockaddr_in addr;
  int addr_len = sizeof(addr);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(8081);
  addr.sin_addr.s_addr = 0;

  if (bind(s, (struct sockaddr *) &addr, addr_len) != 0) {
    perror("bind error");
    return 1;
  }

  if (listen(s, 10)) {
    perror("listen error");
    return 1;
  }

  int client_fd = accept(s, (struct sockaddr *) &addr, (socklen_t *) &addr_len);

  char buffer[256] = {0};
  recv(client_fd, buffer, 256, 0);

  char* file = buffer + 5;
  *strchr(file, ' ') = 0;
  int open_fd = open(file, O_RDONLY);
  sendfile(client_fd, open_fd, 0, (off_t *) 256, NULL, 0);
  close(open_fd);
  close(client_fd);
  close(s);
  return 0;
}
