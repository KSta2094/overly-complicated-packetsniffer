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

int main(int argc, char **argv) {
  char *device = argv[1];
  int fd;
  char *file = "packet";
  char buffer[get_BFD(&fd, device)];
  printf("-----------------\nRUNNING SERVER\n");
  int pfd;
  int afd;
  createSocket(file, &pfd, &afd);
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
    //    for (int i = 0; i != bytes; i++) {
    //      printf("%02x", buffer[i]);
    //    }

    send(afd, buffer, sizeof(buffer), 0);
  }
  close(afd);
  close(fd);
  shutdown(pfd, 2);
  return 1;
}
