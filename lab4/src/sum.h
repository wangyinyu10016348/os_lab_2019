#ifndef SUM_H
#define SUM_H

struct SumArgs {
  int *array;
  int begin;
  int end;
};

void* Sum(const struct SumArgs *args);

#endif