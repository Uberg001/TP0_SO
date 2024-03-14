#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger,"Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();
	
	ip=config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO");
	valor=config_get_string_value(config,"CLAVE");

	log_info(logger,valor);
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

	printf("END!");
}

t_log* iniciar_logger(void){
	t_log* nuevo_logger=log_create("/home/utnso/tp0/client/tp0.log","test_log",1,LOG_LEVEL_INFO);

	if(nuevo_logger==NULL){
		perror("Something went wrong");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void){
	t_config* nuevo_config=config_create("/home/utnso/tp0/client/cliente.config");
	if(nuevo_config==NULL){
		perror("Something went wrong");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger){
	char* leido;

	//primera lectura
	leido = readline("> ");
	log_info(logger,leido);

	//el resto...
	while(strcmp(leido,"")!=0){
		free(leido);
		leido = readline("> ");
		log_info(logger,leido);
	}

	free(leido);
}

void paquete(int conexion){
	char* leido;
	t_paquete* paquete=crear_paquete();

	leido = readline("> ");
	while(strcmp(leido,"")!=0){
			agregar_a_paquete(paquete, leido, strlen(leido)+1);
			free(leido);
			leido = readline("> ");
		}

	free(leido);
	
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
