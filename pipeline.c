#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int fd[2]; // fd[0] = leitura, fd[1] = escrita
    pid_t pid;

    if (pipe(fd) == -1) { // Cria o pipe
        perror("Erro ao criar o pipe");
        exit(1);
    }

    pid = fork(); // Cria um processo filho

    if (pid < 0) { // Erro ao criar o processo
        perror("Erro ao criar o processo filho");
        exit(1);
    }

    if (pid > 0) { // Processo pai - Produtor
        close(fd[0]); // Fecha leitura no produtor

        char mensagem[50];
        for (int i = 1; i <= 5; i++) {
            sprintf(mensagem, "Mensagem %d", i);
            write(fd[1], mensagem, strlen(mensagem) + 1);
            printf("Produtor: enviou '%s'\n", mensagem);
            sleep(1);
        }

        close(fd[1]); // Fecha escrita
        wait(NULL);
    } 
    else { // Processo filho - Consumidor
        close(fd[1]); // Fecha escrita no consumidor

        char buffer[50];
        while (read(fd[0], buffer, sizeof(buffer)) > 0) {
            printf("Consumidor: recebeu '%s'\n", buffer);
        }

        close(fd[0]); // Fecha leitura
        exit(0);
    }

    return 0;
}
