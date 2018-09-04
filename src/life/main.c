#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

int count_lines(char * archivo){
	FILE* input_file = fopen(archivo, "r");
	int count = 0;  // Line counter (result)
	char c;  // To store a character read from file
	// Check if file exists
	if (input_file == NULL)
	{
	    printf("Could not open file %s", archivo);
	    return 0;
	}
	// Extract characters from file and store in character c
	for (c = getc(input_file); c != EOF; c = getc(input_file))
	    if (c == '\n') // Increment count if this character is newline
	        count = count + 1;

	// Close the file
	fclose(input_file);
	return count;
}

int main(int argc, char** argv)
{

	int quantum = 3;
	if (argc > 4)
	{
		printf("Modo de uso: ./scheduler input.txt output.csv 5\n");
		return 0;
	}
	else if (argc < 3){
		printf("Ingrese archivos de input.txt y output.csv para correr el programa\n");
		return 0;
	}
	else if (argc == 4){
		quantum = atoi(argv[3]);
	}

	printf("QUANTUM: %i\n", quantum);

	FILE* input_file = fopen(argv[1], "r");

	int num = count_lines(argv[1]);
	printf("NUMERO DE LINEAS %i\n\n", num);

	/* Falló la apertura del archivo */
	if(!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}

	//######################################
	//###    LECTURA DEL ARCHIVO TXT     ###
	//######################################

	// Basado en esta página https://rosettacode.org/wiki/Read_a_file_line_by_line
	// LECTURA INPUT.TXT linea por linea
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	if (input_file == NULL)
		exit(EXIT_FAILURE);

	int position = 0;
	while ((read = getline(&line, &len, input_file)) != -1){
		Proceso * proceso = malloc(sizeof(Proceso));
		proceso -> ID = position;
		position++;


		printf("%s", line);
		char* token = strtok(line, " ");
		proceso_init(proceso, token);
		token = strtok(NULL, " ");

		proceso -> tiempo_llegada = atoi(token);
		token = strtok(NULL, " ");
		proceso -> num_etapas = atoi(token);
		token = strtok(NULL, " ");
		proceso -> time_list = queue_init();

		while (token != NULL){
			token = strtok(NULL, " ");
		}

	}
	free(line);


	//######################################
	//##   SIMULACION DE LOS PROCESOS     ##
	//######################################


	fclose(input_file);

	return 0;
}
