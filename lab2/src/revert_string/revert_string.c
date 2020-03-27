#include "revert_string.h"
#include <string.h>
void RevertString(char *str)
{
	for (int i = 0, l = strlen(str); i < l / 2; i++) 
    {
		char c = str[i];
		str[i] = str[l - i - 1];
		str[l - i - 1] = c;
    }
}

