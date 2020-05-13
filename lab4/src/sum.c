#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  size_t sum = 0;
  // TODO: your code here
  //printf("\n\n");
  for (int i = args->begin; i < args->end; i++) {
      //printf("%d ", args->array[i]);
      sum += args->array[i];
  }
  //printf("\nPartial sum: %lu ", sum);
  return sum;
}