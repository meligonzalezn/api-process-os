

/*
 Primer punto de API procesos
 1871071 - Michelle González Hernández 
 1832127 - Melissa González Nebrijo 

*/


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define READ 0
#define WRITE 1

//Función que se encarga de dividir los comandos, recibe la cadena de caracteres
//que los contiene y el array donde se almacena el resultado

void  split(char string[], char *stringdiv[]){
 const char delimiter[] = " "; //se hace uso de un delimitador para saber donde separar las cadenas 
 char *token = strtok(string, delimiter);  
 int n = 0;
 while(token != NULL)
 {
  stringdiv[n] = token;
  n++;
  token = strtok(NULL, delimiter);//Por medio del while se sigue recorriendo la cadena 
                                  // y se entrega NULL a la función para que el resultado de strtok no sea siempre el mismo
  stringdiv[n] = NULL;
 }
}

//Funcion main

int main(int argc, char* argv[]) {

 pid_t pid;
 int fd[2];
 if (pipe(fd) == -1) {
  perror("Creating pipe");
  exit(EXIT_FAILURE);
 }
 pid = fork();
 if (pid == 0){                                                                                                                                                                                                                                        // The child process will execute wc.
  //Esto es ejecutado por el hijo
  close(fd[WRITE]);//Se cierra el descriptor de escritura del pipe
  dup2(fd[READ], STDIN_FILENO); //Apunta al descriptor que devuelve el pipe 
  char *cm[3]; 
  cm[2] = NULL;
  // se hace uso de la funcion split para separar por palabras el comando y los argumentos
  split(argv[2], cm);
  //Se ejectua el comando con sus argumentos y la entrada que retorna el padre
  execvp(cm[0], cm);
 }
 else if (pid == -1){
  perror("fork() failed");
  exit(EXIT_FAILURE);
 }
 else{
  //Esto es ejecutado por el padre
  close(fd[READ]); // se cierra el descriptor de lectura del pipe
  dup2(fd[WRITE], STDOUT_FILENO); //Redirecciona la salida de escritura al pipe
  char *comand[3]; 
  comand[2] = NULL;
  split(argv[1], comand);
  //Se ejecuta el comando con los argumentos
  execvp(comand[0], comand);
 }
}

/*
Pruebas: 

El programa se puede probar de la siguiente manera: 

./first "cat p4.c" "wc"
./first "cat p4.c" "wc -l"
./first "cat p4.c" "wc -w"
*/
