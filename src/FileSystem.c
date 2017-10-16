#include "FileSystem.h"

void main() {
	char * PUERTO;
	logFs = log_create("FileSystem.log", "FileSystem", 0, 0);
	FILE * directorios =
			fopen(
					"/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/directorios.dat",
					"w+");
	FILE * nodos =
			fopen(
					"/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/nodo.bin",
					"wb");
	tablaNodos = list_create();
	tablaArchivos = list_create();
	tablaDeNodos * aux = malloc(sizeof(tablaDeNodos));
	pthread_t hiloConsola;
	configFs =
			config_create(
					"/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/FileSystem.config");
	PUERTO = config_get_string_value(configFs, "PUERTO");

	crearBitmap();
	pthread_create(&hiloConsola, NULL, IniciarConsola, NULL);
	pthread_join(hiloConsola, NULL);
	soyServidor(PUERTO);
	fclose(directorios);
	fclose(nodos);
	log_destroy(logFs);
}

void partir() {
	FILE * archivo = fopen("datos.txt", "r+");
	int fd = fileno(archivo);
	int tamano;
	struct stat buff;
	fstat(fd, &buff);
	tamano = buff.st_size;

	void * archivoABytes = malloc(buff.st_size);
	void * aux = malloc(1048576);

	while (tamano <= 0) {
		if (tamano > 0 && tamano < 1048576) {
			fread(aux, tamano, 1, archivo);
			break;
		} else {
			fread(aux, 1048576, 1, archivo);
			tamano -= 1048576;
		}
	}

	free(aux);
	free(archivoABytes);
}

void almacenarArchivo(char * PATH, char * nombreArchivo, int * TipoArchivo,
		FILE * archivo) {
	if (TipoArchivo == 1) {
		puts("soy archivo binario");
	} else {
		puts("soy archivo de texto");
	}
}
void leerArchivo(char * PATH) {
	puts("leo archivo yamafs");
}

soyServidor(char * puerto) {
	void *get_in_addr(struct sockaddr *sa) {
		if (sa->sa_family == AF_INET) {
			return &(((struct sockaddr_in*) sa)->sin_addr);
		}

		return &(((struct sockaddr_in6*) sa)->sin6_addr);
	}
	int i;
	int nuevoSocket;
	int SocketYama;
	int SocketMASTER_WORKER;
	int quiensoy;
	int fdmax;
	struct sockaddr_storage remoteaddr;
	int listener = crearSocketYBindeo(puerto);
	struct addrinfo hints, *ai, *p;
	fdmax = listener;
	socklen_t addrlen;
	fd_set master;
	fd_set SocketsDataNodes;
	FD_ZERO(&master);
	FD_ZERO(&SocketsDataNodes);
	FD_SET(listener, &master);
	escuchar(listener);

	bool estable;
	estable = false;

							pthread_attr_t attr;
							pthread_t hilo_atenderDatas ;
	while (1) {
		SocketsDataNodes = master;
		if (select(fdmax + 1, &SocketsDataNodes, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &SocketsDataNodes)) {
				if (i == listener) {
					addrlen = sizeof remoteaddr;
					nuevoSocket = accept(listener,
							(struct sockaddr *) &remoteaddr, &addrlen);

					if (nuevoSocket == -1) {
						perror("accept");
					} else {
						FD_SET(nuevoSocket, &master);
						recv(nuevoSocket, &quiensoy, sizeof(int), MSG_WAITALL); //Devuelve -1 en error
						switch (quiensoy) {

						case yama: {
							if(estable)
							{
								SocketYama = nuevoSocket;
							puts("conectado");
							}
							else{
								puts("todavia no es estado estable");
							}
						}
							break;
						case master2: {
							SocketMASTER_WORKER = nuevoSocket;
							//hilo
						}
							break;
						case datanode: {

							//Hilos detachables cpn manejo de errores tienen que ser logs
							int  res;

							res = pthread_attr_init(&attr);
							res = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
							res = pthread_create (&hilo_atenderDatas,&attr,atenderData, (void *)&nuevoSocket);
							pthread_attr_destroy(&attr);
							estable = true;
						}
							//abrirhilo
							break;
						}
						if (nuevoSocket > fdmax) {
							fdmax = nuevoSocket;
						}
					}
				} else {
					void * stream;
					Mensaje mensaje = recibirMensaje(i, &stream);
					if (mensaje.accion <= 0) {
						if (mensaje.accion == 0) {
							printf("Se murio socket %d hung up\n", i); ///hacer log
						} else {
							perror("Hubo un error que no deberia pasar");
						}
						close(i);
						FD_CLR(i, &master);
					} else {
						if (i == SocketYama) {
							//cosas que haga yama
							switch (mensaje.accion) {
							case INT:{
								printf("%d",*(int*)stream);
							}
							break;
							case ARCHIVO: {

							}
								break;
							case CADENA: {
							}
								break;
							}
						} else {
							//soy datanode
						}

					}
				}
			}
		}
	}
}

void crearBitmap() {
	int FileD;
	FILE* archivoDeBitmap =
			fopen(
					"/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/metada.bin",
					"r+");
	if (archivoDeBitmap == NULL) {
		log_info(logFs,
				"[Configurar Todo]-Se tuvo que crear un bitmap nuevo, ya que no habia un bitmap anterior.");
		archivoDeBitmap =
				fopen(
						"/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/metada.bin",
						"w+");
		int cantidad = ceil(((double) 24) / 8.0);
		char* cosa = string_repeat('\0', cantidad);
		fwrite(cosa, 1, cantidad, archivoDeBitmap);
		free(cosa);
	} else {
		log_info(logFs, "Se cargo el bitmap al FileSystem");
	}
	fclose(archivoDeBitmap);
	FileD = open("/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata",
	O_RDWR);

	struct stat scriptMap;
	fstat(FileD, &scriptMap);

	char* bitmap2 = mmap(0, scriptMap.st_size, PROT_WRITE | PROT_READ,
	MAP_SHARED, FileD, 0);
	bitMap = bitarray_create(bitmap2, ceil(((double) 24) / 8.0));
	log_info(logFs, "[Configurar Todo]-Se creo correctamente el bitmap");
	puts("anda");
}

void *atenderData(void *argu) {
	int socket = (int)argu;

		void * stream;
		Mensaje mensaje = recibirMensaje(socket, &stream);
		switch (mensaje.accion) {
		case INT:
			printf("%d\n",*(int*)stream);
		}
}
