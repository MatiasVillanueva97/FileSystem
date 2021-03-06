
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
#include <math.h>
#include <commons/collections/list.h>


//File System

//Tabla de Directorios
typedef struct{
  int index;
  char nombre[255];
  int padre;
}tablaDeDirectorios;

//Tabla de Nodos
typedef struct {
	char* nodo;
	uint32_t total;
	uint32_t libre;
} ContenidoXNodo;

typedef struct {
	uint32_t tamanio;
	uint32_t libres;
	t_list * nodo;
	t_list * contenidoXNodo;
}tablaDeNodos;

//Tabla de Archivos

typedef struct {
	char* nodo;
	uint32_t bloque;
	uint32_t bytes;
}BloqueCopia;

typedef struct{
	int tamanio;
	int tipo;
	t_list* bloqueCopia;
}tablaArchivo;

//info para yama---------------------
typedef struct {
	int nodo;
	int bloque;
}BloqueCopiaParaArchivo;

typedef struct {
	int numeroDeBloque;
	BloqueCopiaParaArchivo copia1;
	BloqueCopiaParaArchivo copia2;
	double finArchivo;
}Bloque;

typedef struct{
	uint32_t tamaniobytes;
	uint32_t tipo;
	uint32_t directorioPadre;
	uint32_t estado;
	Bloque bloque;
}archivoEnFileSystem;
//----------
//Bitmap por Nodo
typedef struct  {
	char* nodo;
	t_bitarray 	*bitarray;
} tablaBitmapXNodos;

//Variables

tablaDeDirectorios tablaDirectorios[100];
t_list * tablaArchivos;
t_list * tablaNodos;
t_list * tabaBitmapXNodo;
t_list * listaDeBitMap;
tablaDeNodos * tablaNodosGlobal;

t_log * logFs;
t_config * configFs;
t_bitarray arrayDebits;
t_config * configFs;
t_bitarray* bitMap ;


