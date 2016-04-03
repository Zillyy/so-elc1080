#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){
	clock_t t_ini, t_fim;
	pid_t pid;
	int status;

	printf("Sou o P1, PID = %d, PPID = %d\n", getpid(), getppid());
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
		else if(pid == 0){   //P3
			printf("Sou o P3, PID = %d, PPID = %d\n", getpid(), getppid());
			pid = fork();

			if(pid == -1){
				fprintf(stderr, "Fork falhou\n");
				exit(-1);
			}
			else if(pid == 0){   //P4
				printf("Sou o P4, PID = %d, PPID = %d\n", getpid(), getppid());
				pid = fork();

				if(pid == -1){
					fprintf(stderr, "Fork falhou\n");
					exit(-1);
				}
				else if(pid == 0){   //P5
					printf("Sou o P5, PID = %d, PPID = %d\n", getpid(), getppid());
					pid = fork();

					if(pid == -1){
						fprintf(stderr, "Fork falhou\n");
						exit(-1);
					}
					else if(pid == 0){   //P6
						printf("Sou o P6, PID = %d, PPID = %d\n", getpid(), getppid());
						pid = fork();

						if(pid == -1){
						fprintf(stderr, "Fork falhou\n");
						exit(-1);
						}
						else if(pid == 0){   //P7
							printf("Sou o P7, PID = %d, PPID = %d\n", getpid(), getppid());
							exit(1);
						}
						else{
							wait(&status);
							exit(1);
						}
					}
					else{
						wait(&status);
						exit(1);
					}
				}
				else{
					wait(&status);
					exit(1);
				}
			}
			else{
				wait(&status);
				exit(1);
			}
		}
		else{
			wait(&status);
			exit(1);
		}
	}
	else{
		wait(&status);
		t_fim = clock();
		printf("Tempo de execucao: %f segundos\n",((float)(t_fim - t_ini)/CLOCKS_PER_SEC));
		exit(1);
	}
	exit(0);
}