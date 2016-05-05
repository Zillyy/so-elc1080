#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

static pid_t cria_fork(void){
	pid_t pid;

	if((pid = fork()) < 0){
		fprintf(stderr, "Fork de pid %d falhou\n", getpid());
		exit(-1);
	}
	return pid;
}

void cria_processos(int num_filhos){
	int i = 0;
    for (i = 0; i < num_filhos; i++){
        if (cria_fork() == 0){
            continue;
        }
        break;
    }

    fprintf(stderr, "Tenho PID = %d e PPID = %d\n", getpid(), getppid());

    while (wait(0) > 0);
    sleep(1.5);

    fprintf(stderr, "Final: %d\n", getpid());
    
    return;
}

int main(){
	clock_t t_ini, t_fim;
	t_ini = clock();

	pid_t aux = getpid();
	fprintf(stderr, "Inicio: %d\n", aux);
	
	cria_processos(6);
	
	if(getpid() == aux){
		t_fim = clock();
		printf("Tempo de execucao: %f segundos\n",((float)(t_fim - t_ini)/CLOCKS_PER_SEC));
	}

    return 0;
}