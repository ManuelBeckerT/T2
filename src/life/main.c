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
	Cell ** cells_array;

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
	printf("\nEXECUTION TIME: %i\n", execution_time);

	FILE* input_file = fopen(argv[1], "r");
	/* Falló la apertura del archivo */
	if(!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}

	int execution_time = atoi(argv[2]);
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
		tablero -> name = name;
		printf("%s\n", tablero -> name);
		int cell_count;
		fscanf(input_file, "%i ", &cell_count);
		tablero -> cell_count = cell_count;
		printf("%i\n", tablero -> cell_count);
		tablero -> cells_array = malloc(sizeof(Cell *)*cell_count);
		tablero -> position = 0;

		Cell ** cells_array = tablero -> cells_array;
		while (cell_count > 0){
			Cell * cell = malloc(sizeof(Cell));
			cells_array[tablero -> position] = cell;
			tablero -> position ++;
			int x, y;

			if (cell_count == 1){
				fscanf(input_file, "%i %i\n", &x, &y);
				cell -> x = x;
				cell -> y = y;
			}
			else{
				fscanf(input_file, "%i %i ", &x, &y);
				cell -> x = x;
				cell -> y = y;
			}
			printf("%i %i\n", cell -> x, cell -> y);
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
		Cell ** array = current_tablero -> cells_array;
		for (int i = 0; i < current_tablero -> cell_count; i++){
			printf("CELL POSITION %i, %i\n", array[i] -> x, array[i] -> y);
			free(array[i]);
		}
		free(array);
		free(current_tablero);
	}
	free(tableros);


	fclose(input_file);

	return 0;
}
