#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
int main() {

  char *file = "packet";
  char buffer[128];
  int *res = createSocket(file, buffer);
  int fd = res[0];
  int afd = res[1];
  // Clear buffer
  memset(buffer, 0, sizeof(buffer));

  // Read from accepted socket
  while (1) {
    ssize_t n = read(afd, buffer, sizeof(buffer) - 1);

    if (strstr(buffer, (char *)"END")) {

      printf("Broken connection\n");
      char msg[] = "CLOSING SOCKET";
      send(afd, &msg, sizeof(msg), 0);
      break;
    };
    if (n > 0) {
      buffer[n] = '\0';
      printf("Received: %s\n", buffer);
    }
    send(afd, &buffer, sizeof(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
  }
  close(afd);

  shutdown(fd, 2);
  return 1;
}
