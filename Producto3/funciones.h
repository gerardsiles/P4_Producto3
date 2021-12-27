#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#pragma once
#define _GNU_SOURCE
//tamano del bugger para leer
#define BUFFER_SIZE 256
#define IP_SIZE 30

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


// Funcion para mostrar por pantalla el contenido de un archivo
void imprimirArchivo(FILE *input) {
	char ip[IP_SIZE] = "";
	while ((fgets(ip, IP_SIZE, input) != NULL)) {
		printf("%s", ip);
		//fputs(ip, copiaArchivo);
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

			if (strstr(ch, "IPv4") != NULL) {
				sscanf(ch, "%*[^:]: %[^\n]", dns);
			}
		}		
	}
	if (encontrado == 0) {
		puts("el dns introducido no se ha encontrado");
		return -1;
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
void comprobarConexionIp(FILE** archivo) {
	FILE* ipsConConexion = NULL;
	char ch[BUFFER_SIZE] = "";
	char ip[30] = "";
	int paquetesRecibidos = 5, encontrado = 0, lineasContadas = -1;
	ipsConConexion = fopen("ipsConConexion.txt", "w+");
	if (ipsConConexion == NULL) {
		puts("Se ha producido un error. Reinicie el programa.");
		return -1;
	}

	while ((fgets(ch, sizeof(ch), archivo)) != NULL) {
		if (strstr(ch, "Ping") && (encontrado == 0)) {
			sscanf(ch, "%*[^0-9]%s", &ip);
			encontrado = 1;
		}
		if (strstr(ch, "Received")) {
			sscanf(ch, "%*[^=]=%*[^=]=%d", &paquetesRecibidos);
		}

		if (strcmp(ch, "\n") == 0 ) {
			lineasContadas++;
		}
		/* 
		al terminar de leer la informacion del dns, procedemos a grabarlo en un fichero
		si ha habido conexion con la ip
		*/
		if (lineasContadas == 2) {
			if (paquetesRecibidos > 0) {
				fputs(strcat(ip, "\n"), ipsConConexion);
			} 
			// reiniciar variables
			lineasContadas = 0;
			encontrado = 0;
			strcpy(ip, "");
		}
	}
	fclose(ipsConConexion);
}

void conexionMasRapida() {

}