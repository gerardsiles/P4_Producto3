#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

#define BUFFER_SIZE 256


int main() {
	FILE* archivoIP = NULL;
	FILE* ipResponde = NULL;
	FILE* ipconfig = NULL;
	char rutaArchivo[100] = "";
	char nombreDNS[150] = "";
	char DNS[30] = "";
	char DNSMasRapido[30] = "";



	// Recibir la ruta del archivo con las ip
	// DNSips.txt
	// // C:\Users\gerar\source\repos\Producto3\Producto3\DNSips.txt
	// Wireless LAN adapter Wi-Fi
	do {
		printf("Introduzca la direccion del archivo:\n");
		leeCad(rutaArchivo, 100);
		archivoIP = fopen(rutaArchivo, "r");
		if (archivoIP != NULL) {
			puts("/--------------------------/");
			printf("Contenido del archivo" ANSI_COLOR_GREEN " %s.\n" ANSI_COLOR_RESET, rutaArchivo);
			puts("/--------------------------/");
			// mostrar contenido del archivo por pantalla
			imprimirArchivo(archivoIP);
		}
		else {
			printf(ANSI_COLOR_RED "No se pudo abrir el archivo, comprueba que la ruta sea correcta.\n" ANSI_COLOR_RESET);
			puts("/--------------------------/");
		}
	} while (archivoIP == NULL);
	fclose(archivoIP);

	// Preguntar por el adaptador de red
	puts("/--------------------------/");
	printf("Introduce el nombre del adaptador que quieres conocer el DNS\n");
	puts("/--------------------------/");
	leeCad(nombreDNS, 150);

	// Conseguimos la informacion de los conectores de red donde esta conectado
	system("ipconfig/all > \"ipconfig_all.txt\"");
	ipconfig = fopen("ipconfig_all.txt", "r");

	// Comprobar que el archivo de texto con la configuracion de los DNS se haya creado con exito
	if (ipconfig == NULL) {
		printf(ANSI_COLOR_RED "Ha surgido un error, reinicia el programa.\n" ANSI_COLOR_RESET);
		return -1;
	}

	// Mostrar configuracion actual del DNS a traves del ipconfig
	encontrarDNS(ipconfig, &nombreDNS, &DNS);
	printf("El DNS actual es" ANSI_COLOR_YELLOW " %s" ANSI_COLOR_RESET "\n", DNS);
	// Con la informacion del DNS conseguida, ya podemos cerrar el archivo de ipconfig
	fclose(ipconfig);



	// comprobar si las ip son accesibles
	printf("Comprobando que las ip son accesibles, esto puede tardar un rato.\n");
	archivoIP = fopen(rutaArchivo, "r");
	if (archivoIP != NULL) {
		lanzarPing(archivoIP);
	}
	else {
		puts(ANSI_COLOR_RED "No se pudo abrir el archivo. Reinicie el programa." ANSI_COLOR_RESET);
		exit(1);
	}
	fclose(archivoIP);
	printf("ips comprobadas. Gracias por la espera.\n");

	// reutilizamos la variable FILE
	archivoIP = fopen("pingsip.txt", "r");
	if (archivoIP == NULL) {
		printf("No se pudo abrir el archivo con la informacion de las ip, reinicia el programa.\n");
		return -1;
	}

	puts("comprobando si las ip respondieron.");
	puts("/--------------------------/");
	comprobarConexionIp(archivoIP, DNS);
	puts("Comprobacion terminada.");
	// cerrar el archivo
	fclose(archivoIP);

	// Comprobar cual de los dns es el mas rapido en el archivo temporal
	// Abrir el archivo
	puts("/--------------------------/");
	puts("Comprobando que conexion es la mas rapida");
	puts("/--------------------------/");

	ipResponde = fopen("ipsConConexion.txt", "r");
	if (ipResponde == NULL) {
		printf(ANSI_COLOR_RED "No se pudo abrir el archivo con la informacion de las ip, reinicia el programa.\n" ANSI_COLOR_RESET);
		return -1;
	}
	// comprobamos la velocidad de las ip que han dado respuesta
	lanzarPing(ipResponde);
	fclose(ipResponde);

	archivoIP = fopen("pingsip.txt", "r");
	if (archivoIP == NULL) {
		puts(ANSI_COLOR_RED "Se ha producido un error. Reinicie el programa." ANSI_COLOR_RESET);
		return -1;
	}
	
	conexionMasRapida(archivoIP, DNSMasRapido);
	fclose(archivoIP);

	// Cambiar si es mas rapido
	if (strcmp(DNS, DNSMasRapido) != 0) {
		puts("prueba");
	}
	else {
		puts("El DNS introducido es el mas rapido, no se van a aplicar ningun cambio.");
	}

	// Eliminar del sistema los archivos creados durante la ejecucion del programa
	puts("Procediendo a borrar los archivos de texto generados durante la ejecucion");
	puts("/--------------------------/");

	if (remove("ipconfig_all.txt") == 0) {
		printf("Archivo ipconfig_all.txt borrado.\n");
	}
	else {
		printf("El archivo ipconfig_all.txt no se ha podido borrar o no existe.\n");
	}
	if (remove("ipsConConexion.txt") == 0) {
		printf("Archivo ipsConConexion.txt borrado.\n");
	}
	else {
		printf("El archivo ipsConConexion.txt no se ha podido borrar o no existe.\n");
	}
	if (remove("pingsip.txt") == 0) {
		printf("Archivo pings.txt borrado.\n");
	}
	else {
		printf("El archivo pings.txt no se ha podido borrar o no existe.\n");
	}
	return 0;
}




