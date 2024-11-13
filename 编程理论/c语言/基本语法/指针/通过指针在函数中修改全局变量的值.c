#include <stdio.h>

int real = 0;

void mmm(int a) {
  int *ptr;
  ptr = &real;
  *ptr = a;
}

int main() {
  mmm(3);
  printf("%d", real);
}
