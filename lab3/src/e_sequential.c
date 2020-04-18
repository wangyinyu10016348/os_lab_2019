#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    printf("\nLaunching sequential_min_max with parameters:\n");
    printf("seed: 42\narray_size: 6\n\n");
    
    int pid = fork();
    if (pid > 0) {
        printf("Done!\n");
        wait(NULL);
    }
    else if ( pid == 0 ) {
		execl("sequential_min_max", "./sequential_min_max",
		"42", "6", NULL);
	}
	else {
	    printf("\nProgram launching failed!\n");
	    exit(EXIT_FAILURE);
	}
    
    return EXIT_SUCCESS;
}