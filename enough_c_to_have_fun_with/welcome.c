#include <stdio.h>

int make_it_higher(int n) {
  return n + 100;
}

int main() {
  int name = 0;

  while (1) {
    name = make_it_higher(name);
    printf("welcome to %04X\n", name);
  }
}
