#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	int status;
	printf("Sou o P1, PID = %d, PPID = %d\n", getpid(), getppid());
	pid = fork();

	if(pid == -1){
		fprintf(stderr, "Fork falhou\n");
		exit(-1);
	}
	else if(pid == 0){   //P2
		printf("Sou o P2, PID = %d, PPID = %d\n", getpid(), getppid());

		pid = fork();

		if(pid == -1){
			fprintf(stderr, "Fork falhou\n");
			exit(-1);
		}
		else if(pid == 0){   //P3
			printf("Sou o P3, PID = %d, PPID = %d\n", getpid(), getppid());
			printf("P3 dormindo por 10 segundos!\n");
			sleep(10);
			printf("P3 resumindo execucao!\n");
			exit(1);
		}
		else{		//P2
			printf("P2 dormindo por 10 segundos!\n");
			sleep(10);
			printf("P2 resumindo execucao!\n");
			wait(&status); //espera por P3
			exit(1);
		}
	}
	else{
		printf("P1 dormindo por 10 segundos!\n");
		sleep(10);
		printf("P1 resumindo execucao!\n");
		wait(&status);
		exit(1);
	}
}
