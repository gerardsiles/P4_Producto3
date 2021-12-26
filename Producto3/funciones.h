#define _CRT_SECURE_NO_WARNINGS

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

int contienePalabra(char a[], char b[]) {
	if (strstr(a, b) != NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

// Funcion para leer contenido de un archivo
int leerArchivo(char nombreArchivo[], FILE** archivo) {
	int error = 0;
	if ((*archivo = fopen(nombreArchivo, "r")) == NULL) {

		error = 1;
	}
	return error;
}

// Funcion para mostrar por pantalla el contenido de un archivo
void imprimirArchivo(FILE *input) {
	char ip[IP_SIZE] = "";
	while ((fgets(ip, IP_SIZE, input) != NULL)) {
		printf("%s", ip);
		//fputs(ip, copiaArchivo);
	}
	printf("\n");
}

void encontrarDNS(char nombre[]) {
	//TODO
	system("ipconfig > \"ipconfig.txt\"");
}
