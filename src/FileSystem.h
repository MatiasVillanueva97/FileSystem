
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../../Shared/Consola.c"
#include "../../Shared/Socket.c"
#include <pthread.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//File System
#include <commons/collections/list.h>
//Tabla de Directorios
typedef struct{
  int index;
  char nombre[255];
  int padre;
}tablaDeDirectorios;
//Tabla de Nodos
typedef struct {
	int nodoTotal;
	int nodoLibre;
}listaDeNodos;

typedef struct {
	int tamanioTotal;
	int tamanioLibre;
	listaDeNodos * nodos;
}tablaDeNodos;

//Archivo en FileSystem
typedef struct {
	int nodo;
	int bloque;
}BloqueCopia;

typedef struct {
	int numeroDeBloque;
	BloqueCopia copia1;
	BloqueCopia copia2;
	double finArchivo;
}Bloque;

typedef struct{
	int tamanio;
	int tipo;
	int directorioPadre;
	int estado;
	Bloque bloque;
}archivoEnFileSystem;

typedef struct{
	BloqueCopia Copia1;
	BloqueCopia Copia2;
	double FinBloque;
}ListaBLoques;


//Tabla de Archivos

typedef struct{
	int tamanio;
	int tipo; // 1 texto 0 binario
	ListaBLoques * listaDeBloques;
}tablaDeArchivos;
//Variables
tablaDeDirectorios tablaDirectorios[100];
t_list * tablaArchivos;
t_list * tablaNodos;
t_log * logFs;
t_config * configFs;
t_bitarray arrayDebits;
t_config * configFs;
t_bitarray* bitMap ;
//prototipos
void almacenarArchivo(char * PATH,char * nombreArchivo ,int * TipoArchivo,FILE * archivo);
void leerArchivo(char * PATH);
soyServidor(char * PUERTO);
void partir() ;
void *atenderData(void * argu);

