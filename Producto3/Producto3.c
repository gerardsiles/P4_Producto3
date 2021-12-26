#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

#define BUFFER_SIZE 256


int main() {
	FILE* archivo = NULL;
	FILE* ipconfig = NULL;
	char rutaArchivo[100] = "";
	char nombreDNS[30] = "";
	char ch[BUFFER_SIZE] = "";

	// Recibir la ruta del archivo con las ip
	// TEST C:\Users\gerar\source\repos\Producto3\Producto3\copia_archivo.txt
	do {
		printf("Introduzca la direccion del archivo:\n");
		leeCad(rutaArchivo, 100);
		if (leerArchivo(rutaArchivo, &archivo) != 1) {
			puts("/--------------------------/");
			printf("Contenido del archivo %s.\n", rutaArchivo);
			puts("/--------------------------/");
			// mostrar y copiar contenido del archivo
			imprimirArchivo(archivo);
		}
		else {
			printf("No se pudo abrir el archivo, comprueba que la ruta sea correcta.\n");
			puts("/--------------------------/");
		}
	} while (archivo == NULL);

	// Preguntar por el adaptador de red
	puts("/--------------------------/");
	printf("Introduce el nombre del adaptador que quieres conocer el DNS\n");
	puts("/--------------------------/");
	leeCad(nombreDNS, 30);
	encontrarDNS(nombreDNS);

	// Mostrar configuracion actual del DNS a traves del ipconfig
	// Comprobar que el archivo de texto con la configuracion de los DNS se haya creado con exito
	if (leerArchivo("ipconfig.txt", "r") == NULL) {
		printf("Ha surgido un error, reinicia el programa.\n");
		return 0;
	}

	// Buscar el dns introducido por el usuario
	while ((fgets(ch, BUFFER_SIZE, ipconfig)) != NULL) {
		if (contienePalabra(ch, nombreDNS)) {
			if (contienePalabra(ch, "ipv4")) {
				scanf();
			}
		}
	}
	printf("El DNS seleccionado es %s");
	// comprobar si las ip son accesibles
	// si son accesibles, escribirlas en un archivo temporal
	// Comprobar cual de los dns es el mas rapido entre los del dnsips y el archivo temporal
	return 0;
}




