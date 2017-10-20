#include "filesystem.h"

void main() {
	char * PUERTO;
	char * IP;
	logFileSystem = log_create("logFile.log", "FILESYSTEM", false, LOG_LEVEL_TRACE);
	FILE * directorios =fopen("/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/directorios.dat","w+");
	FILE * nodos =fopen("/home/utnso/workspace/tp-2017-2c-Usleep/FileSystem/metadata/nodo.bin","wb");
	//creo todas las listas de estructuras
	tablaNodos = list_create();
	tablaArchivos = list_create();
	tablaNodosGlobal=malloc(sizeof(tablaDeNodos));
	tablaNodosGlobal->nodo=list_create();
	tablaNodosGlobal->contenidoXNodo=list_create();
	//configuro cosas
	char *nameArchivoConfig = "configFilesystem.txt";
   	if (leerArchivoConfig(nameArchivoConfig, keysConfigFS, datosConfigFS)) {	//leerArchivoConfig devuelve 1 si hay error
   			printf("Hubo un error al leer el archivo de configuración");
   			return 0;
   		}
	PUERTO = datosConfigFS[PUERTO_PROPIO];
	IP = datosConfigFS[IP_PROPIA];
	//abro el hilo de consola
	pthread_t hiloConsola;
	//levanto consola
	pthread_create(&hiloConsola, NULL, IniciarConsola, NULL);

	//levanto servidor
	soyServidor(PUERTO);
	pthread_join(hiloConsola, NULL);//dejo que termine el hilo
	fclose(directorios);
	fclose(nodos);
	log_destroy(logFileSystem);
}
//---Deberiamos hacer algun tipo de handshake y usar este enum
enum modulo{
	datanode = 1,
	worker = 2,
	yama = 3,
	moduloMaster = 4 ,
	filesystem = 5
};
soyServidor(char * PUERTO, char *IP) {
	int i;
	int nuevoSocket;
	int SocketYama;
	int SocketWorer;
	int quiensoy; //falta
	int fdmax;
	// 2º) inicializar server y aguardar conexiones (de datanode)
   	int listener=inicializarServer(IP, PUERTO);
	if(listener<0){
		log_error(logFileSystem,"No pude iniciar como servidor");
		puts("No pude iniciar como servidor");
		return EXIT_FAILURE;
	}
	//puts("Ya estoy preparado para recibir conexiones\n");
	fdmax = listener;
	fd_set master;
	fd_set SocketsDataNodes;
	FD_ZERO(&master);
	FD_ZERO(&SocketsDataNodes);
	FD_SET(listener, &master);

	bool estable;
	estable = false; //falta especificar como cabiamos estable aa true
	estable = true; // lo dejamos estable hasta que hagamos las conexiones de los datanodes

	while (1) {
		SocketsDataNodes = master;
		if (select(fdmax + 1, &SocketsDataNodes, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &SocketsDataNodes)) {
				if (i == listener) {
					nuevoSocket = aceptarConexion(listener);
					if(nuevoSocket<0){
						log_error(logFileSystem,"Hubo un error al aceptar conexiones");
						puts("Hubo un error al aceptar conexiones\n");
						return EXIT_FAILURE;
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
						case worker: {
							SocketWorker = nuevoSocket;
							//atender a worker,supongo que almacenar el archivo
						}
							break;
						case datanode: {
							//necesito recibir informacion del datanode,id de datanode y bitmap para la proxima entrega
							crearBitmapDeNodosConectados("Nodo329");//donde dice Nodo329 iria el id del datanode
							estable = true;
						}
							break;
						}
						if (nuevoSocket > fdmax) {
							fdmax = nuevoSocket;
						}
					}
				} /*else { Lo de aca para abajo esto sirve para cuando ya estan conectados ,
						si te envian mensajes la funcion
						recibirmensaje era parte de mi serializacion*//*
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
				}*/
			}
		}
	}
}





//--------------------------------------
//crea bitmap de cada datanode que se conecta si se fijan en el select en necesario que le pasen la id del nodo (el nombre)
void crearBitmapDeNodosConectados(char * NodoConectado){
	t_list * nodosConectados=list_create();

	list_add(nodosConectados,NodoConectado);
	void crearBitmapXNodo(char * nodoConectado){

		char * PATH_bitmap_xNOdo=string_new();
		string_append(&PATH_bitmap_xNOdo,"/home/utnso/FileSystem/metadata/bitmaps/");
		string_append(&PATH_bitmap_xNOdo,nodoConectado);
		string_append(&PATH_bitmap_xNOdo,".dat");
		crearBitmap(PATH_bitmap_xNOdo,nodoConectado);

	}
	list_iterate(nodosConectados,crearBitmapXNodo);

}

//una funcion que trabaja con la anterior
void crearBitmap(char * PATH ,char * nodoConectado){
	int FileD;
	FILE* archivoDeBitmap = fopen(PATH,"r+");
	if(archivoDeBitmap == NULL){
		log_info(logFileSystem,"Se tuvo que crear un bitmap nuevo [%s], ya que no habia un bitmap anterior.",nodoConectado);
		archivoDeBitmap =fopen(PATH,"w+");
		int cantidad = 1048576;
		char* cosa = string_repeat('\0',cantidad);
		fwrite(cosa,1,cantidad,archivoDeBitmap);
		free(cosa);
	}
	else{
		log_info(logFileSystem,"Se cargo el bitmap [%s] al FileSystem ",nodoConectado);
	}
	fclose(archivoDeBitmap);
	FileD = open(PATH,O_RDWR);


	struct stat scriptMap;
	fstat(FileD, &scriptMap);

	char* bitmap2 = mmap(0, scriptMap.st_size, PROT_WRITE |PROT_READ , MAP_SHARED, FileD,  0);
	bitMap= bitarray_create(bitmap2,1048576);
	log_info(logFileSystem,"[Configurar Todo]-Se creo correctamente el bitmap [%s]",nodoConectado);

}

//funcion de almacenar un archivo
void almacenarArchivo(char * PATH, int TipoArchivo){

	if (TipoArchivo == 1) {//si es 1 es binario
		partirArchivoBinario(PATH);
	} else if{
		(TipoArchivo ==0{// es texto
		partiArchivoDeTexto(PATH);}

	}
}


//partir el archivo de texto
partiArchivoDeTexto(char * PATH){
	FILE*archivo=fopen(PATH,"r+");
	int fd=fileno(archivo);
	struct stat buff;
	fstat(fd,&buff);
	char caracter;
	int a;
	int ultimoBarraN=0;
	int ultimoBarraNAnterior=0;
	int i= 1;
	int ultimoBarraNAntesDeMega=0;
	void * contenidoAEnviar;
	int * posicionBarraN;
	int * posicionArchivoTerminado;

t_list * posiciones =list_create();

		while(!feof(archivo)){// hace un while uno agarrando caracter por caracter y se fija el ultimo \n antes de llegar al mega y que la palabra no quede cortada a la mitad

			caracter = fgetc(archivo);

			if(caracter=='\n'){

				a=ftell(archivo);
				ultimoBarraN=ftell(archivo);

			}

				if(ftell(archivo)==1048576+ultimoBarraNAntesDeMega){
					ultimoBarraNAntesDeMega=ultimoBarraN;
					posicionBarraN=malloc(sizeof(int));
					*posicionBarraN=ultimoBarraN;
					list_add(posiciones,posicionBarraN);
					ultimoBarraNAnterior=ultimoBarraN;
			}

			else{

			}
		}//while


		fseek(archivo,0,SEEK_END);

		posicionArchivoTerminado=malloc(sizeof(int));

		*posicionArchivoTerminado=ftell(archivo);

		list_add(posiciones,posicionArchivoTerminado);

		fseek(archivo,0,SEEK_SET);

		int posicionActual = 0;

		void partir(int * posicion){
			if(posicionActual==0){
			contenidoAEnviar=malloc(*posicion);
			fread(contenidoAEnviar,*posicion,1,archivo);
			//Aca deberiamos hacer un send al datanode el inconveniente es que cada vez que entre aca va a hacer un send si el archivo pensa 8 megas y tiene que entrar 8 veces haria 8 sends

			}else {
				int posicionAnterior=*((int*)list_get(posiciones,posicionActual-1));
				contenidoAEnviar=malloc((*posicion)-posicionAnterior);
				fread(contenidoAEnviar,(*posicion)-posicionAnterior,1,archivo);

				//aca pasaria lo mismo que el comentario de arriba

			}
			posicionActual++;
		fclose(archivo2);
		}
		list_iterate(posiciones,partir);
		fclose(archivo);
		free(contenidoAEnviar);
}

// parte los archivos binarios
void partirArchivoBinario(char* PATH){
	FILE* archivo = fopen(PATH, "r+");
	int fd = fileno(archivo);
	int tamano;
	struct stat buff;
	fstat(fd, &buff);
	tamano = buff.st_size;

	void * archivoABytes = malloc(buff.st_size);
	void * contenidoAEnviar = malloc(1048576);
	while (tamano >= 0) {

		if (tamano > 0 && tamano < 1048576) {
			fread(contenidoAEnviar, tamano, 1, archivo);
			//aca ser haria el send ,pero pasa lo mismo que en los archivos de texto,dependiendo de cuantos megas es cuantos sends hace
			break;
		} else {
			fread(contenidoAEnviar, 1048576, 1, archivo);
			//aca paso lo mismo que antes(otro send)
			tamano -= 1048576;
		}

	}

	free(contenidoAEnviar);
	free(archivoABytes);
	fclose(archivo);
}


//esta funcion te sirve saber la cantidad de bloques en la que quedaria partido antes de hacerlo, redondea para arriba, se necesita compilarlo con -lm
//si queda 7megas y 50 te va a dar el resultado 8, creo que esta funcion sirve para ayudarnos a serializar en algun momento los mensajes
int cantidadBloques(char * PATH){
	FILE* archivo=fopen(PATH,"r+");
	int tamanoArchivo =1;
	int cantidadBloques =0;
	fseek(archivo,0,SEEK_END);
	tamanoArchivo=ftell(archivo);
	cantidadBloques = ceil((float)tamanoArchivo/1048576);
	return cantidadBloques;

}
