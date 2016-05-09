#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 1000000

typedef struct {
	int needle;
	int *haystack;
	int init_pos;
	int final_pos;
	int thread_id;
} param_thread;

void *SearchThread(void *args){
	param_thread *param = args;
	int i;
	for(i = param->init_pos; i < param->final_pos + 1; i++){
		if(param->haystack[i] == param->needle){
			printf("Thread %d encontrou o valor %d\n", param->thread_id, param->needle);
			return;
		}		
	}
	printf("Thread %d: 0\n", param->thread_id);
}

//Inicializa array com 1000000 numeros
int *InitArray(){
	int *array = (int *) malloc(sizeof(int) * MAX);
	int i;
	for(i = 0; i < MAX; i++){
		array[i] = i + 1;
	}
	return array;
}

int main(int argc, char *argv[]){
	clock_t t_ini_create, t_fim_create, t_ini_join, t_fim_join, t_ini_total, t_fim_total;
	t_ini_total = clock();
	int i, rc;
	pthread_t threads[atoi(argv[2])];
	int *haystack = InitArray();

	param_thread *param = malloc(sizeof(param_thread) * atoi(argv[2]));

	int incr_value = MAX / atoi(argv[2]);
	int init = 0;
	int final = incr_value - 1;

	//Cria parametros para argv[2] threads de acordo com o argumento [2] passado
	for(i = 0; i < atoi(argv[2]); i++){
		param[i].thread_id = i;
		param[i].needle = atoi(argv[1]);
		param[i].haystack = haystack;
		param[i].init_pos = init;
		param[i].final_pos = final;

		init += incr_value;
		final += incr_value;  
	}
	
	//Loop de criação de threads
	t_ini_create = clock();
	for(i = 0; i < atoi(argv[2]); i++){
		rc = pthread_create(&threads[i], NULL, SearchThread, (void *)&param[i]);                            
   		if (rc){                          
       		printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc);                            
        	exit(-1);                          
    	}
	}
	t_fim_create = clock();
	printf("Tempo de criacao de threads: %f segundos\n",((float)(t_fim_create - t_ini_create)/CLOCKS_PER_SEC));

	//Loop de join de threads
	t_ini_join = clock();
	for(i = 0; i < atoi(argv[2]); i++){
		pthread_join(threads[i], NULL);   
	}
	t_fim_join = clock();
	printf("Tempo de join de threads: %f segundos\n",((float)(t_fim_join - t_ini_join)/CLOCKS_PER_SEC));
	
	//Desalocando
	free(haystack);
	free(param);
	t_fim_total = clock();
	printf("Tempo total de execucao: %f segundos\n",((float)(t_fim_total - t_ini_total)/CLOCKS_PER_SEC));
	return 0;
}