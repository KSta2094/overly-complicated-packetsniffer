#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int *createSocket(char *file, char buffer[]) {

  int res[2];
  memset(buffer, '0', sizeof(&buffer));
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    printf("ERROR: failed to create socket");
    perror("");
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, file, sizeof(addr.sun_path));
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
  unlink(file);

  if (bind(fd, (struct sockaddr *)&addr, SUN_LEN(&addr)) < 0) {
    printf("ERROR: failed to bind socket");
    perror("");
  }
  if (listen(fd, 10) < 0) {
    printf("ERROR: Failed to listen");
    perror("");
  }
  while (1) {
    int afd = accept(fd, NULL, NULL);
    res[1] = afd;
    if (afd < 0) {
      perror("Failed to receive connection");
      continue;
    }

    res[0] = fd;
    return res;
  }
}
