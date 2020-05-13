#include <unistd.h>
 
int main(void) {
        if (!fork()) {
                /* Потомок немедленно завершается*/
                _exit(0);
        }
    /* Родитель ждет около 30 секунд*/
        sleep(30);
}