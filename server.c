#include <sys/socket.h>
#include <netinet/in.h>
#include <endian.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

int main(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr = {
    AF_INET,
    htobe16(6969),
    0
  };
  
  bind(fd, (struct sockaddr *) &addr, sizeof(addr));
  
  listen(fd, 32);
  
  int client_fd = accept(fd, 0, 0);
  char buffer[256] = {0};

  recv(client_fd, buffer, 256, 0);

  // "GET /file.html ...."
  // "012345..."
  char* f = buffer + 5;
  *strchr(f, ' ') = '\0';
  int opened_fd = open(f, O_RDONLY);
  sendfile(client_fd, opened_fd, 0, 256);
  
  close(opened_fd);
  close(client_fd);
  close(fd);
    
  return 0;
}
