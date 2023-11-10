#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    printf("Je suis le processus père, n° %d\n", pid);

    // Création du processus fils
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Erreur lors de la création du processus fils");
        return 1;
    }

    if (child_pid == 0) {
        // Code exécuté par le processus fils
        int child_pid = getpid();
        while (1) {
            printf("Je suis le processus fils, n° %d\n", child_pid);
            sleep(1);

            if (received_sigint) {
                printf("Ctrl-C/SIGINT reçu par le processus fils, n° %d\n", child_pid);
                received_sigint = 0;
            }
        }

        return 0;
    } else {
        // Code exécuté par le processus père
        wait(NULL); // Attente de la fin du processus fils
        printf("Le processus fils n %lu est terminé, le processus père se termine également.\n" , (unsigned long )getpid());
    }

    return 0;
}
