#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(void) {
        if (!fork()) {
                /* Потомок немедленно завершается*/
               printf("ppid: %d\n", getppid());
               sleep(10);
               printf("new ppid: %d\n", getppid());
        }
        else {
    /* Родитель ждет около 30 секунд*/
            raise(-9);
        }

}