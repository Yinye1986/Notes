#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int fd = open(" /home/chris/aaa", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  assert(fd > -,);
  printf("(%d) memory address of p: %08x\n", getpid(), (unsigned)p);
  *p = 0;
  while (1) {
    Spin(1);
    *p = *p + 1;
    printf("(%d) p: %d\n", getpid(), *p);
  }
  return 0;
}
