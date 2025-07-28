

#define BPF_DEVICE "/dev/bpf0"
#define INTERFACE_NAME "en0"

int get_BFD(int *fd,char * device);

void createSocket(char *file,  int *fd, int *afd) ;
