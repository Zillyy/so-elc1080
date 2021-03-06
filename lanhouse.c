#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_PC 3
#define LIVRE 0
#define EM_USO 1

//Criacao dos semaforos
sem_t sem_pcs;
sem_t mutex_lan;

//Area compartilhada
typedef struct{
	int pcs[NUM_PC];
	int fila_de_espera;
} lanhouse;
lanhouse lan; 

//Struct usada para saber qual o id da thread
typedef struct{
	int cliente_id;
} param_cliente;

//Retorna o indice do pc
int verifica_pcs(){
	int i;
	for(i = 0; i < NUM_PC; i++){
		if(lan.pcs[i] == LIVRE) return i;
	}
}

void *Lanhouse(void *arg){
	param_cliente *cliente = arg;
	
	printf("Cliente [%d] chegando\n", cliente->cliente_id);
	printf("Sala de espera com %d pessoa(s)\n", lan.fila_de_espera);
	sem_wait(&mutex_lan); //Wait lan
	lan.fila_de_espera++; //Incrementa fila de espera
	if(lan.fila_de_espera > 15){
		//Se a fila estiver cheia, manda o cliente embora
		lan.fila_de_espera--;
		sem_post(&mutex_lan);
		printf("Cliente [%d] saindo: Sala de espera lotada\n", cliente->cliente_id);
		return NULL;
	} else{
		sem_post(&mutex_lan); //Post lan

		sem_wait(&sem_pcs); //Wait pcs
		
		sem_wait(&mutex_lan); //Wait lan
		lan.fila_de_espera--; //Tira o cliente da fila de espera
		int i = verifica_pcs(); //Retorna o indice do pc
		lan.pcs[i] = EM_USO; //Informa que pc esta sendo usado
		sem_post(&mutex_lan); //Post lan

		printf("PC [%d] sendo usado pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		usleep(10000 * (8 + (rand()%2)));
		printf("PC [%d] esta livre pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		
		sem_wait(&mutex_lan); //Wait lan
		lan.pcs[i] = LIVRE; //Informa que pc esta livre
		sem_post(&mutex_lan); //Post lan
		
		sem_post(&sem_pcs); //Post pcs
		
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

	//Inicializa RNG
	srand((unsigned)time(NULL));

	int rc, i;
	int num_usuarios = atoi(argv[1]);
	pthread_t usuarios[num_usuarios];
	param_cliente *cliente = (param_cliente *) malloc(sizeof(param_cliente) * num_usuarios);

	//Preenche parametros de cada cliente/thread
	for (i = 0; i < num_usuarios; i++){
		cliente[i].cliente_id = i + 1;
	}

	//Inicializa fila de espera com 0 pessoas
	lan.fila_de_espera = 0;

	//Inicializacao das flags dos pcs
	for (i = 0; i < NUM_PC; i++){
		lan.pcs[i] = LIVRE;
	}

	//Inicializacao dos semaforos
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