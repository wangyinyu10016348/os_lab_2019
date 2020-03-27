#include "swap.h"

void Swap(char *left, char *right)
{
	char tea =*right;
    *right = *left;
    *left = tea;
}
