#include "find_min_max.h"
#include <stdio.h>
#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

///////////////my code
  printf("array: ");
	for (int i = begin; i < end; i++)
	{
		printf("%d ", array[i]);
		
		if (min_max.min > array[i])
			min_max.min = array[i];

		if (min_max.max < array[i])
			min_max.max = array[i];
	}
	printf("\n");
//////////////end
    
  return min_max;
}
