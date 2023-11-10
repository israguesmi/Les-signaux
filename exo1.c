#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s NF NM\n", argv[0]);
        return 1;
    }

    int NF = atoi(argv[1]);  // Nombre de fils
    int NM = atoi(argv[2]);  // Nombre de fois à afficher

    for (int i = 0; i < NF; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {  // Processus fils
            for (int j = 0; j < NM; j++) {
                printf("Activite rang %d : identifiant = %d\n", i, getpid());
            }
            exit(i);
        }
    }

    for (int i = 0; i < NF; i++) {
        int status;
        pid_t terminated_pid = wait(&status);
        if (WIFEXITED(status)) {
            int retour = WEXITSTATUS(status);
            printf("Valeur retournée par le fils %d = %d\n", terminated_pid, retour);
        }
    }

    return 0;
}
