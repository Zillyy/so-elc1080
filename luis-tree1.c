#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){
	clock_t t_ini, t_fim;
	pid_t pid;
	int status;

	//---------------------LADO ESQUERDO DA ARVORE-------------------
	t_ini = clock();
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
		else if(pid == 0){   //P4
			printf("Sou o P4, PID = %d, PPID = %d\n", getpid(), getppid());
			exit(1);
		}
		else{		//P2
			wait(&status); //espera por P4
			pid = fork();

			if(pid == -1){
				fprintf(stderr, "Fork falhou\n");
				exit(-1);
			}
			else if(pid == 0){   //P5
				printf("Sou o P5, PID = %d, PPID = %d\n", getpid(), getppid());
				exit(1);
			}
			else{		//P2
				wait(&status); //espera por P5
				exit(1);
			}
		}
		exit(1);
	}
	//---------------------LADO DIREITO DA ARVORE----------------------
	else{		//P1
		printf("Sou o P1, PID = %d, PPID = %d\n", getpid(), getppid());
		wait(&status); //espera por P2
		pid = fork();

		if(pid == -1){
			fprintf(stderr, "Fork falhou\n");
			exit(-1);
		}
		else if(pid == 0){   //P3
			printf("Sou o P3, PID = %d, PPID = %d\n", getpid(), getppid());
			pid = fork();

			if(pid == -1){
				fprintf(stderr, "Fork falhou\n");
				exit(-1);
			}
			else if(pid == 0){   //P6
				printf("Sou o P6, PID = %d, PPID = %d\n", getpid(), getppid());
				exit(1);
			}
			else{		//P3
				wait(&status); //espera por P6
				pid = fork();

				if(pid == -1){
					fprintf(stderr, "Fork falhou\n");
					exit(-1);
				}
				else if(pid == 0){   //P7
					printf("Sou o P7, PID = %d, PPID = %d\n", getpid(), getppid());
					exit(1);
				}
				else{		//P2
					wait(&status); //espera por P7
					exit(1);
				}
			}
		}
		else{		//P1
			wait(&status); //espera por P3
			t_fim = clock();
			printf("Tempo de execucao: %f segundos\n",((float)(t_fim - t_ini)/CLOCKS_PER_SEC));
			exit(1);
		}
	}
	exit(0);
}