#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <net/bpf.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

void createSocket(char *file, int *fd, int *afd) {

  *fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (*fd == -1) {
    printf("ERROR: failed to create socket");
    perror("");
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, file, sizeof(addr.sun_path));
  addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
  unlink(file);

  if (bind(*fd, (struct sockaddr *)&addr, SUN_LEN(&addr)) < 0) {
    printf("ERROR: failed to bind socket");
    perror("");
  }
  if (listen(*fd, 10) < 0) {
    printf("ERROR: Failed to listen");
    perror("");
  }
  *afd = accept(*fd, NULL, NULL);
  if (afd < 0) {
    perror("Failed to receive connection");
  }
}

int get_BFD(int *fd, char *device) {

  (*fd) = open(device, O_RDONLY);
  if (fd < 0) {
    perror("could not find device");
    exit(-1);
  }

  struct ifreq ifr;
  strcpy(ifr.ifr_name, INTERFACE_NAME);
  if (ioctl(*fd, BIOCSETIF, &ifr) < 0) {
    perror("Failed to create BIOCSETIF");
    close(*fd);
    exit(-1);
  }
  u_int enable = 1;
  if (ioctl(*fd, BIOCIMMEDIATE, &enable) < 0) {
    perror("Failed to create BIOCIMMEDIATE");
    close(*fd);
    exit(-1);
  }

  u_int buf_len;
  if (ioctl(*fd, BIOCGBLEN, &buf_len) < 0) {
    perror("BIOCGBLEN failed");
    close(*fd);
    exit(1);
  }
  return buf_len;
}
