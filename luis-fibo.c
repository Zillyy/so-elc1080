#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fibo(int n){
	int a = 0, b = 1, aux = 0, i;
	printf("%d ", b);
	for(i = 0; i < n - 1; i++){
		aux = a + b;
		a = b;
		b = aux;
		printf("%d ", aux);
	}
	printf("\n");
	return;
}

int main(int argc, char *argv[]){

	if(argc != 2){
		printf("Uso: %s (numero de elementos da sequencia de Fibonacci) \n", argv[0]);
		exit(-1);
	}

	if(atoi(argv[1]) < 1){
		printf("O numero de termos da sequencia deve ser maior que 0\n");
		exit(-1);
	}

	pid_t pid;
	int status;

	printf("Sou o Pai, PID = %d, PPID = %d\n", getpid(), getppid());
	pid = fork();

	if(pid == -1){
		fprintf(stderr, "Fork falhou\n");
		exit(-1);
	}
	else if(pid == 0){   //FILHO
		printf("Sou o Filho, PID = %d, PPID = %d\n", getpid(), getppid());
		printf("Sequencia de Fibonacci para %s termos: ", argv[1]);
		fibo(atoi(argv[1]));
		exit(1);
	}
	else{		//PAI
		wait(&status); //espera pelo filho
		printf("Sou o Pai, PID = %d, PPID = %d\n", getpid(), getppid());
		exit(1);
	}
}