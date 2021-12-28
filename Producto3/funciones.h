#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#pragma once
#define _GNU_SOURCE
//tamano del bugger para leer
#define BUFFER_SIZE 256
#define IP_SIZE 30
// Colores para los mensajes en pantalla
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Funcion para leer strings
int leeCad(char* cad, int n) {
	int i, c;
	c = getchar();

	if (c == EOF) {
		cad[0] = '\0';
		return 0;
	}
	if (c == '\n')
		i = 0;
	else {
		cad[0] = c;
		i = 1;
	}
	for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++)
		cad[i] = c;
	cad[i] = '\0';
	if (c != '\n' && c != EOF)
		while ((c = getchar()) != '\n' && c != EOF);
	return 1;
};


// Funcion para mostrar por pantalla el contenido de un archivo con una serie de ips
void imprimirArchivo(FILE *input) {
	char ip[IP_SIZE] = "";

	while ((fgets(ip, IP_SIZE, input) != NULL)) {
		printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, ip);
	}
	printf("\n");
}

// Funcion que, dado un adaptador de red, encuentra su DNS
void encontrarDNS(FILE *archivo,char *nombreDNS,char *dns) {
	char ch[BUFFER_SIZE] = "";
	int numeroDeLineasVacias = 0;
	int encontrado = 0;

	while (((fgets(ch, BUFFER_SIZE, archivo)) != NULL) && numeroDeLineasVacias != 2) {
		// Buscar la primera ocurrencia del dns

		if ((strstr(ch, nombreDNS) != NULL) || (encontrado == 1)) {
			encontrado = 1;
			if (strcmp(ch, "\n") == 0) {
				numeroDeLineasVacias++;
			}

			if (strstr(ch, "DNS Servers") != NULL) {
				sscanf(ch, "%*[^:]: %[^\n]", dns);
			}
		}		
	}
	if ((encontrado == 0) || (strlen(dns) == 0)) {
		puts(ANSI_COLOR_RED "el adaptador de red introducido no se ha encontrado, comprueba que el nombre introducido sea el del adaptador." ANSI_COLOR_RESET);
		exit(1);
	}
}

// Funcion para lanzar pings a una serie de ip y redireccionarlo a un archivo
void lanzarPing(FILE** input) {
	int ping = 0;
	if (input != NULL) {
		char comandoPing[80] = "ping ";
		char ip[30] = "";

		while ((fgets(ip, sizeof(ip), input)) != NULL) {
			// Remover \n del final de la ip
			ip[strcspn(ip, "\n")] = 0;
			// Si es la primera ocurrencia, creamos el archivo
			if (ping < 1) {
				strcat(strcat(comandoPing, ip), " > \"pingsip.txt\"");
				system(comandoPing);
				ping++;
			}
			else {
				// si no es el primer ping, reiniciar string y reconstruir el comando
				strcat(strcat(strcat(strcpy(comandoPing, ""), "ping "), ip), "  >> \"pingsip.txt\"");
				system(comandoPing);
			}
		}
	}
}

// Funcion que comprueba si las ip anteriores han tenido conexion
void comprobarConexionIp(FILE** archivo, char *dns) {
	FILE* ipsConConexion = NULL;
	char ch[BUFFER_SIZE] = "";
	char ip[30] = "";
	int paquetesRecibidos = 5, encontrado = 0, escaneado = 0;
	ipsConConexion = fopen("ipsConConexion.txt", "w+");
	if (ipsConConexion == NULL) {
		puts(ANSI_COLOR_RED "Se ha producido un error. Reinicie el programa." ANSI_COLOR_RESET);
		return -1;
	}

	while ((fgets(ch, sizeof(ch), archivo)) != NULL) {
		if (strstr(ch, "Ping") && (encontrado == 0)) {
			sscanf(ch, "%*[^0-9]%s", &ip);
			encontrado = 1;
		}
		if ((strstr(ch, "Received") || strstr(ch, "Recibido"))) {
			sscanf(ch, "%*[^=]=%*[^=]=%d", &paquetesRecibidos);
			escaneado = 1;
		}
		/* 
		al terminar de leer la informacion del dns, procedemos a grabarlo en un fichero
		si ha habido conexion con la ip
		*/
		if (escaneado == 1){
			if (paquetesRecibidos > 0) {
				printf("La ip" ANSI_COLOR_YELLOW " %s" ANSI_COLOR_RESET " ha tenido conexion.\n", ip);
				fputs(strcat(ip, "\n"), ipsConConexion);
			}
			else {
				printf("La ip" ANSI_COLOR_RED " %s" ANSI_COLOR_RESET "  no ha tenido conexion.\n", ip);
			}
			// reiniciar variables
			escaneado = 0;
			encontrado = 0;
			strcpy(ip, "");

		}
	}
	fclose(ipsConConexion);
}

// Funcion que busca la ip con media mas rapida y devuelve la mas rapida de un archivo
void conexionMasRapida(FILE** archivo, char *masRapido) {
	char ch[BUFFER_SIZE] = "";
	char ip[30] = "";
	char dnsMasRapido[30] = "";
	int velocidadMedia = 0, ipMasRapida = 500, encontrado = 0, escaneado = 0;

	while ((fgets(ch, sizeof(ch), archivo)) != NULL) {
		if (strstr(ch, "Ping") && (encontrado == 0)) {
			sscanf(ch, "%*[^0-9]%s", &ip);
			encontrado = 1;
		}

		if ((strstr(ch, "Average") || strstr(ch, "Media"))) {
			sscanf(ch, "%*[^=]=%*[^=]=%*[^=]=%d", &velocidadMedia);
			escaneado = 1;
		}

		// si hemos terminado de leer una ip, reiniciamos variables
		if (escaneado == 1) {
			printf("El DNS" ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET " tiene una velocidad media de %d ms.\n", ip, velocidadMedia);
			// comprovar las velocidades
			if (ipMasRapida > velocidadMedia) {
				strcpy(dnsMasRapido, ip);
				ipMasRapida = velocidadMedia;
				printf("El DNS " ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET " es el mas rapido con una velocidad media de %d\n", dnsMasRapido, ipMasRapida);
			}
			else {
				printf("El DNS " ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET " sigue siendo el mas rapido con una velocidad media de %d\n", dnsMasRapido, ipMasRapida);

			}
			// reiniciar variables
			escaneado = 0;
			encontrado = 0;
			strcpy(ip, "");
		}
	}
	strcpy(masRapido, dnsMasRapido);
}