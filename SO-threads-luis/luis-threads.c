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
	for(i = param->init_pos; i < param->final_pos; i++){
		if(param->haystack[i] == param->needle){
			printf("Thread achou o numero %d\n", param->needle);
			return;
		}		
	}
	printf("Thread nao achou o numero %d\n", param->needle);
}

int *InitArray(){
	int *array = (int *) malloc(sizeof(int) * MAX);
	int i;
	for(i = 0; i < MAX; i++){
		array[i] = i + 1;
	}
	return array;
}

int main(int argc, char *argv[]){
	clock_t t_ini, t_fim;
	int i, rc;
	pthread_t threads[5];

	param_thread *param = malloc(sizeof(param_thread));
	param->needle = 10000;
	param->haystack = InitArray();
	param->init_pos = 0;
	param->final_pos = 100000;
	
	t_ini = clock();
	
	
	for(i = 0; i < 5; i++){
		param->thread_id = i;
		rc = pthread_create(&threads[i], NULL, SearchThread, (void *)&param);                            
   		if (rc){                          
       		printf("ERROR; return code from pthread_create() is %d\n", rc);                            
        	exit(-1);                          
    	}
    	
	}
	for(i = 0; i < 5; i++){
		pthread_join(threads[param->thread_id], NULL);   
	}

	t_fim = clock();
	printf("Tempo de execucao: %f segundos\n",((float)(t_fim - t_ini)/CLOCKS_PER_SEC));
	free(param->haystack);	
	return 0;
}