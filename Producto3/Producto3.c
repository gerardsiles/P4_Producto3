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
	FILE* ipconfig = NULL;
	char rutaArchivo[100] = "";
	char nombreDNS[150] = "";
	char DNS[30] = "";

	// Recibir la ruta del archivo con las ip
	// TEST C:\Users\gerar\source\repos\Producto3\Producto3\DNSips.txt
	do {
		printf("Introduzca la direccion del archivo:\n");
		leeCad(rutaArchivo, 100);
		archivoIP = fopen(rutaArchivo, "r");
		if (archivoIP != NULL) {
			puts("/--------------------------/");
			printf("Contenido del archivo %s.\n", rutaArchivo);
			puts("/--------------------------/");
			// mostrar y copiar contenido del archivo
			imprimirArchivo(archivoIP);
		}
		else {
			printf("No se pudo abrir el archivo, comprueba que la ruta sea correcta.\n");
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
		printf("Ha surgido un error, reinicia el programa.\n");
		return -1;
	}

	// Mostrar configuracion actual del DNS a traves del ipconfig
	encontrarDNS(ipconfig, &nombreDNS, &DNS);
	printf("El DNS actual es %s\n", DNS);
	// Con la informacion del DNS conseguida, ya podemos cerrar el archivo de ipconfig
	fclose(ipconfig);



	// comprobar si las ip son accesibles
	printf("Comprobando que las ip son accesibles, esto puede tardar un rato.\n");
	archivoIP = fopen(rutaArchivo, "r");
	if (archivoIP != NULL) {
		lanzarPing(archivoIP);
		fclose(archivoIP);
	}
	printf("ips comprobadas. Gracias por la espera.\n");

	// reutilizamos la variable FILE
	archivoIP = fopen("pingsip.txt", "r");
	if (archivoIP == NULL) {
		printf("No se pudo abrir el archivo con la informacion de las ip, reinicia el programa.\n");
		return -1;
	}

	puts("comprobando si las ip respondieron.");
	puts("/--------------------------/");
	comprobarConexionIp(archivoIP);
	puts("Comprobacion terminada.");
	fclose(archivoIP);

	// Comprobar cual de los dns es el mas rapido en el archivo temporal
	// Abrir el archivo
	// lanzar funcion por cada ip
	// Actualizar si es mas rapido
	// cerrar el archivo
	// Cambiar si es mas rapido
	// borrar archivos generados

	// Eliminar del sistema los archivos creados durante la ejecucion del programa
	puts("Procediendo a borrar los archivos de texto generados durante la ejecucion");
	puts("/--------------------------/");

	//if (remove("ipconfig_all.txt") == 0) {
	//	printf("Archivo ipconfig_all.txt borrado.\n");
	//}
	//else {
	//	printf("El archivo ipconfig_all.txt no se ha podido borrar o no existe.\n");
	//}
	//if (remove("ipsConConexion.txt.txt") == 0) {
	//	printf("Archivo ipsConConexion.txt borrado.\n");
	//}
	//else {
	//	printf("El archivo ipsConConexion.txt no se ha podido borrar o no existe.\n");
	//}
	return 0;
}




