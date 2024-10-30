#include <stdio.h>

int main() {
  unsigned int bytes[10];

  for (int i = 0; i < 10; i += 1) {
    printf("value: %d\n", bytes[i]);
  }
}
