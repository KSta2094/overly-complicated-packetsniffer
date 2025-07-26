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

#define BPF_DEVICE "/dev/bpf0"
#define INTERFACE_NAME "en0"

int get_BFD(int *fd) {

  (*fd) = open(BPF_DEVICE, O_RDONLY);
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

int main() {
  int fd;
  char *file = "packet";
  u_char buffer[get_BFD(&fd)];
  printf("-----------------\nRUNNING SERVER\n");
  int *res = createSocket(file, buffer);
  int pfd = res[0];
  int afd = res[1];
  memset(buffer, 0, sizeof(buffer));
  // Read from accepted socket
  //
  while (1) {
    int bytes = read(fd, buffer, sizeof(buffer));
    if (bytes == -1) {
      perror("Failed to read");
      close(fd);
      exit(1);
    }
    send(afd, buffer, sizeof(buffer), 0);
  }
  close(afd);
  close(fd);
  shutdown(pfd, 2);
  return 1;
}
