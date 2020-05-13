#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

void sighandler(int signum) {
   printf("Caught signal %d, coming out...\n", signum);
   kill(0, SIGKILL);
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = 10;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            
            if (seed <= 0) {
                printf("Invalid arguments (seed)!\n");
                exit(EXIT_FAILURE);
            }         
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0)
            {
                printf("Invalid arguments (array_size)!\n");
                exit(EXIT_FAILURE);
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0)
            {
                printf("Invalid arguments (pnum)!\n");
                exit(EXIT_FAILURE);
            }
            break;
          case 3:
            with_files = true;
            break;

          case 4:
            timeout = atoi(optarg);
            if (signal(SIGALRM, sighandler) == SIG_ERR)
                printf("\ncan't catch SIGINT\n");
            alarm(timeout);
            
            if (timeout <= 0)
            {
                printf("Invalid arguments (timeout)!\n");
                exit(EXIT_FAILURE);
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

int array_part = array_size / pnum;
  int pipefd1[2];
  int pipefd2[2];
  char min_array[24];
  char max_array[24];
  
  if (!with_files) {
      if (pipe(pipefd1) == -1) {
           perror("pipe1");
           exit(EXIT_FAILURE);
       }
       if (pipe(pipefd2) == -1) {
           perror("pipe2");
           exit(EXIT_FAILURE);
       }
  }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        struct MinMax min_max;
        // parallel somehow

        if (i != pnum - 1) {
            min_max = GetMinMax(array, i*(array_part), (i + 1)*(array_part));
        }
        // c++ discards a fractional part of the quotient after dividing operation
        else {
            min_max = GetMinMax(array, i*(array_part), array_size);
        }

        if (with_files) {
          char filename[8];
            snprintf(filename, 8, "file%d", i);
            
            FILE* fp;
            fp = fopen(filename, "w+");
            fprintf(fp, "%d,%d", min_max.min, min_max.max);
            fclose(fp);
        } 
        else {
            snprintf(min_array, 24, "%d", min_max.min);
            snprintf(max_array, 24, "%d", min_max.max);
        
            close(pipefd1[0]); // Close unused read end
            close(pipefd2[0]); // Close unused read end 
            
            write(pipefd1[1], min_array, 24);
            write(pipefd2[1], max_array, 24);
            
            close(pipefd1[1]); // Reader will see EOF (end of file)
            close(pipefd2[1]); // Reader will see EOF (end of file)
        }

        if (timeout != 10)
        {
            while(true) {}
        }

        return 0;
      }

      

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }


     //sleep(timeout);
     //printf("sending signal...\n", index);
    //kill(0, SIGINT);
  while (active_child_processes > 0) {
    if (timeout == 10) {
            wait(NULL); // blocks parent process until any of its children has finished
        }
        else {
            waitpid(0, NULL, WNOHANG);
        }

        active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  char buffer[24];

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
        char filename[8];
        snprintf(filename, 8, "file%d", i);
        
        FILE* fp;
        fp = fopen(filename, "r");
        fscanf(fp, "%d,%d", &min, &max);
        fclose(fp);
        remove(filename);
    } else {
      close(pipefd1[1]);          /* Close unused write end */
      close(pipefd2[1]);          /* Close unused write end */
     
      read(pipefd1[0], buffer, 24);
      min = atoi(buffer);
      
      read(pipefd2[0], buffer, 24);
      max = atoi(buffer);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);

  if (timeout != 10)
  {
    while(true) {}
  }

  return 0;
}