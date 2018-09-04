#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 256

struct Cell
{
	int viva;
	int x;
	int y;
};

typedef struct Cell Cell;

struct Tablero
{
	int size_x;
	int size_y;

	char * name;
	int cell_count;
	int position;

	Cell *** cells_matrix;

};

typedef struct Tablero Tablero;

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

void tablero_init(Tablero * tablero, char * name){
	tablero -> name = calloc(1, sizeof(name) + 1);
	strcpy(tablero -> name, name);
}

int main(int argc, char** argv)
{
	if (argc > 3 || argc < 3){
		printf("Modo de uso ./life <file> <t>\n");
		return 0;
	}

	FILE* input_file = fopen(argv[1], "r");
	/* Falló la apertura del archivo */
	if(!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}

	int execution_time = atoi(argv[2]);
	printf("\nEXECUTION TIME: %i\n", execution_time);

	int table_count, a, b, c, d;
	fscanf(input_file, "%i %i  %i %i %i", &table_count, &a, &b, &c, &d);

	printf("\nINPUT INFO\nTable count: %i\nA: %i\nB: %i\nC: %i\nD: %i\n", table_count, a, b, c, d);

	//######################################
	//###    LECTURA DEL ARCHIVO TXT     ###
	//######################################

	// Basado en esta página https://rosettacode.org/wiki/Read_a_file_line_by_line
	// LECTURA INPUT.TXT linea por linea

	Tablero ** tableros = malloc(sizeof(Tablero *)*table_count);
	int position = 0;

	int input_count = 0;
	while (table_count != input_count){
		printf("\nNUEVO TABLERO\n");
		Tablero * tablero = malloc(sizeof(Tablero));
		tableros[position] = tablero;
		printf("POSITION %i\n", position);
		position ++;

		//printf("%s", line);
		char name[MAX_SIZE];
		fscanf(input_file, "%s ", name);
		tablero -> name = malloc(sizeof(name) + 1);
		strcpy(tablero -> name, name);
		printf("%s\n", tablero -> name);
		int cell_count;
		fscanf(input_file, "%i ", &cell_count);
		tablero -> cell_count = cell_count;
		printf("%i\n", tablero -> cell_count);
		tablero -> position = 0;


		Cell *** array_matrix = malloc(sizeof(Cell **)*d);
		tablero -> cells_matrix = array_matrix;
		for (int i = 0; i < d; i++){
			Cell ** array_line = malloc(sizeof(Cell *)*d);
			array_matrix[i] = array_line;
			for (int j = 0; j < d; j++){
				array_line[j] = malloc(sizeof(Cell));
				array_line[j] -> x = i;
				array_line[j] -> y = j;
				array_line[j] -> viva = 0;
			}
		}


		while (cell_count > 0){
			int x, y;
			if (cell_count == 1){
				fscanf(input_file, "%i %i\n", &x, &y);
				Cell * cell = array_matrix[x][y];
				cell -> x = x;
				cell -> y = y;
				cell -> viva = 1;
			}
			else{
				fscanf(input_file, "%i %i ", &x, &y);
				Cell * cell = array_matrix[x][y];
				cell -> x = x;
				cell -> y = y;
				cell -> viva = 1;
			}
			printf("%i %i\n", x, y);

			cell_count --;
		}
		input_count ++;
	}


	//######################################
	//##   SIMULACION DE LOS PROCESOS     ##
	//######################################
	printf("\nLIBERANDO MEMORIA\n");
	for (int tablero_count = 0; tablero_count < table_count; tablero_count ++){
		Tablero * current_tablero = tableros[tablero_count];
		printf("\nCURRENT TABLERO %s\n", current_tablero -> name);

		// LIBERANDO MATRIX
		Cell *** current_matrix = current_tablero -> cells_matrix;
		for (int i = 0; i < d; i++){
			Cell ** current_line = current_matrix[i];
			for (int j = 0; j < d; j++){
				free(current_line[j]);
			}
			free(current_line);
		}
		free(current_matrix);

		char * current_name = current_tablero -> name;
		free(current_name);
		free(current_tablero);
	}
	free(tableros);


	fclose(input_file);

	return 0;
}
