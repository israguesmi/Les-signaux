#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t received_sigint = 0;

void sigint_handler(int signum) {
    received_sigint = 1;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de la configuration du gestionnaire de signal");
        return 1;
    }

    int pid = getpid();

    while (1) {
        printf("Je suis le processus %d\n", pid);
        sleep(1);

        if (received_sigint) {
            printf("Ctrl-C/SIGINT reçu par le processus de n° %d\n", pid);
            received_sigint = 0;
        }
    }

    return 0;
}
