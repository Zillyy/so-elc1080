#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_PC 3
#define LIVRE 0
#define EM_USO 1

//Criacao do semaforo
sem_t sem_pcs;
sem_t mutex_lan;

//Área compartilhada
typedef struct{
	int pcs[NUM_PC];
	int fila_de_espera;
} lanhouse;
lanhouse lan; 

typedef struct{
	int cliente_id;
} param_cliente;

//verifica se há pcs disponiveis e retorna o indice do pc desligado ha mais tempo
int verifica_pcs(){
	int i;
	for(i = 0; i < NUM_PC; i++){
		if(lan.pcs[i] == LIVRE){
			return i;
		}
	}
}

void *Lanhouse(void *arg){
	param_cliente *cliente = arg;

	printf("Cliente [%d] chegando\n", cliente->cliente_id);
	//sem_wait(&mutex_lan);
	printf("Sala de espera com %d pessoa(s)\n", lan.fila_de_espera);
	lan.fila_de_espera++;
	//sem_post(&mutex_lan);
	
	if(lan.fila_de_espera > 15){
		//sem_wait(&mutex_lan);
		lan.fila_de_espera--;
		printf("Cliente [%d] saindo: Sala de espera lotada\n", cliente->cliente_id);
		//sem_post(&mutex_lan);
		return NULL;
	} else{
		sem_wait(&sem_pcs);
		//sem_wait(&mutex_lan);
		lan.fila_de_espera--;
		int i = verifica_pcs();
		lan.pcs[i] = EM_USO;
		//sem_post(&mutex_lan);
		printf("PC [%d] sendo usado pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		usleep(10000 * (8 + (rand()%2)));
		printf("PC [%d] esta livre pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		//sem_wait(&mutex_lan);
		lan.pcs[i] = LIVRE;
		//sem_post(&mutex_lan);
		sem_post(&sem_pcs);
		
		return NULL;
	}
}

int main(int argc, char *argv[]){
	//Verificacoes de parametros 
	if(argc != 2){
		printf("Uso: %s (numero de usuarios)\n", argv[0]);
		exit(-1);
	}
	if(atoi(argv[1]) < 1){
		printf("O numero de usuarios deve ser maior ou igual a 1!\n");
		exit(-1);
	}

	srand((unsigned)time(NULL));

	int rc, i;
	int num_usuarios = atoi(argv[1]);
	pthread_t usuarios[num_usuarios];
	param_cliente *cliente = (param_cliente *) malloc(sizeof(param_cliente) * num_usuarios);

	//Preenche parametros de cada cliente/thread
	for (i = 0; i < num_usuarios; i++){
		cliente[i].cliente_id = i + 1;
	}

	lan.fila_de_espera = 0;

	//Inicializacao das flags dos pcs
	for (i = 0; i < NUM_PC; i++){
		lan.pcs[i] = LIVRE;
	}
	sem_init(&sem_pcs, 0, 3);
	sem_init(&mutex_lan, 0, 1);

	//Criacao das threads dos usuarios
	for(i = 0; i < num_usuarios; i++){	
		usleep(10000 * (1 + (rand()%4)));
		rc = pthread_create(&usuarios[i], NULL, Lanhouse, (void *)&cliente[i]);                            
   		if (rc){                
       		printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc);                            
        	exit(-1);                          
    	}
	}

	//Join das threads dos usuarios
	for(i = 0; i < num_usuarios; i++){
		pthread_join(usuarios[i], NULL);   
	}
	printf("Lanhouse fechando...\n");

	free(cliente);
	return 0;
}