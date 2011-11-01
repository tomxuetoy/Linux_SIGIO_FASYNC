#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
 
int gotdata=0;
void sighandler(int signo)
{
    if (signo==SIGIO)
        gotdata++;
    return;
}
 
char buffer[4096];
 
int main(int argc, char **argv)
{
    int count;
    struct sigaction action;
 
    memset(&action, 0, sizeof(action));
    action.sa_handler = sighandler;
    action.sa_flags = 0;

    //associate the signal and its handler function
    sigaction(SIGIO, &action, NULL);  //SIGIO: I/O now possible, SIGIO is the key enabler

    fcntl(STDIN_FILENO, F_SETOWN, getpid());  //F_SETOWN: Get owner (process receiving SIGIO); the owner process if current process
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);
 
    while(1) {
        /* this only returns if a signal arrives */
        sleep(86400); /* one day */

        if (!gotdata)
            continue;
        count=read(0, buffer, 4096);
        /* buggy: if avail data is more than 4kbytes... */
        write(1,buffer,count);
        gotdata=0;
    }
}
