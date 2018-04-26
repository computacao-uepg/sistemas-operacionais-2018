#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CADEIRAS 5                /* n�mero de cadeiras para os clientes � espera */
#define TRUE 1

sem_t cliente;                  /* n�mero de cliente que estao esperando atendimento */
sem_t barbeiro;                  /* n�mero de barbeiros � espera de clientes */
sem_t mutex;                    /* para exclus�o m�tua */
int esperando = 0;                /* clientes que est�o esperando */



void cortando();
void cliente_chegou();
void sendoatendido();
void desistiu();

int main() {
sem_init(&cliente, TRUE, 0);
sem_init(&barbeiro, TRUE, 0);
sem_init(&mutex, TRUE, 1);

pthread_t b, c;

 /* criando barbeiro */
 pthread_create(&b, NULL, (void *) barbeiro, NULL);


 /* cria��o de clientes */
 while(TRUE) {
pthread_create(&c, NULL, (void *) cliente, NULL);
sleep(1);
}

return 0;
}

void* barbeiro(void *arg) {
while(TRUE) {
sem_wait(&cliente);   /* vai dormir se o n�mero de clientes for 0 */
sem_wait(&mutex);       /* obt�m acesso a 'esperando' */
esperando = esperando - 1;  /*decrementa o contador de clientes � espera */
sem_post(&barbeiro);     /* um barbeiro est� agora pronto para cortar cabelo */
sem_post(&mutex);       /* libera 'esperando' */
cortando();             /* corta o cabelo (fora da regi�o cr�tica) */
}

pthread_exit(NULL);
}

void* cliente(void *arg) {
sem_wait(&mutex);           /* entra na regi�o cr�tica */

if(esperando < CADEIRAS) {      /* se n�o houver cadeiras vazias, v� embora */
cliente_chegou();
esperando = esperando + 1;  /* incrementa o contador de clientes que estao esperando  */
sem_post(&cliente);   /* Se precisar, acorda o barbeiro */
sem_post(&mutex);       /* libera o acesso a 'esperando' */
sem_wait(&barbeiro);     /* vai dormir se o n�mero de barbeiros livres for 0 */
sendoatendido();          /* cliente est� sentado e sendo atendido pelo barbeiro */
} else {
sem_post(&mutex);       /* a barbearia est� cheia, o cliente n�o espera e vai embora */
desistiu();

}

pthread_exit(NULL);
}

void cortando() {
printf("Barbeiro cortando cabelo de um cliente!\n");
sleep(3);
}

void cliente_chegou() {
printf("Um cliente chegou\n");
}
void sendoatendido() {
printf("Cliente sendo atendido\n");
}

void desistiu() {
printf("O cliente foi embora porque a barbearia esta cheia\n");
}
