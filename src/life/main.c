#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 256

struct Cell
{
	int viva;
	int before;
	int x;
	int y;
};

typedef struct Cell Cell;

struct Tablero
{
	int a;
	int b;
	int c;
	int d;
	int execution_time;

	char * name;
	int cell_count;
	int position;

	Cell *** cells_matrix;

	int next;
	Cell **** states;

};

typedef struct Tablero Tablero;

void print_tablero(Tablero * tablero){
	printf("PRINTING TABLERO %s\n", tablero -> name);
	Cell *** matrix = tablero -> cells_matrix;
	int size = tablero -> d;
	for (int j = 0; j < size; j++){
		printf("| %i | ", j);
		for (int i = 0; i < size; i++){
			if (matrix[size - j - 1][i] -> viva == 1){
				printf("\u25A0 ");
			}
			else{
				printf("\u25A1 ");
			}
			//printf("%i ", matrix[size - j - 1][i] -> viva);
		}
		printf("\n");
	}
	printf("\n");
}

void print_tablero_position(Tablero * tablero, int x, int y){
	printf("PRINTING TABLERO %s\n", tablero -> name);
	Cell *** matrix = tablero -> cells_matrix;
	int size = tablero -> d;
	for (int j = 0; j < size; j++){
		for (int i = 0; i < size; i++){
			if (j == y && i == x){
				printf("\u25A3 ");
			}
			else{
				if (matrix[j][i] -> viva == 1){
					printf("\u25A0 ");
				}
				else{
					printf("\u25A1 ");
				}

			}

			//printf("%i ", matrix[size - j - 1][i] -> viva);
		}
		printf("\n");
	}
	printf("\n");
}

int check_position(Cell *** matrix, int x, int y){
	if (matrix[y][x] -> viva == 1){
		return 1;
	}
	return 0;
}

int count_neighbour_cells(Cell *** matrix, int x, int y, int d){
	int count = 0;
	if (x == 0 && y == 0){ // SI ES LA ESQUINA SUPERIOR IZQUIERDA
		//printf("ESQUINA SUPERIOR IZQUIERDA\n");
		count += check_position(matrix, x + 1, y);
		count += check_position(matrix, x, y + 1);
		count += check_position(matrix, x + 1, y + 1);
	}
	else if (x == 0 && y == d - 1){ // SI ES LA ESQUINA INFERIOR IZQUIERDA
		//printf("ESQUINA INFERIOR IZQUIERDA\n");
		count += check_position(matrix, x + 1, y);
		count += check_position(matrix, x, y - 1);
		count += check_position(matrix, x + 1, y - 1);
	}
	else if (x == d - 1 && y == 0){ // SI ES LA ESQUINA SUPERIOR DERECHA
		//printf("ESQUINA SUPERIOR DERECHA\n");
		count += check_position(matrix, x - 1, y);
		count += check_position(matrix, x, y + 1);
		count += check_position(matrix, x - 1, y + 1);
	}
	else if (x == d - 1 && y == d - 1){ // SI ES LA ESQUINA INFERIOR DERECHA
		//printf("ESQUINA INFERIOR DERECHA\n");
		count += check_position(matrix, x - 1, y);
		count += check_position(matrix, x, y - 1);
		count += check_position(matrix, x - 1, y - 1);
	}
	else if (x == 0){ // SI ES LA PRIMERA COLUMNA
		//printf("PRIMERA COLUMNA\n");
		count += check_position(matrix, x, y - 1);
		count += check_position(matrix, x, y + 1);
		count += check_position(matrix, x + 1, y - 1);
		count += check_position(matrix, x + 1, y + 1);
		count += check_position(matrix, x + 1, y);
	}
	else if (x == d - 1){ // SI ES LA ULTIMA COLUMNA
		//printf("ULTIMA COLUMNA\n");
		count += check_position(matrix, x, y - 1);
		count += check_position(matrix, x, y + 1);
		count += check_position(matrix, x - 1, y - 1);
		count += check_position(matrix, x - 1, y + 1);
		count += check_position(matrix, x - 1, y);
	}
	else if (y == 0){ // SI ES LA PRIMERA LINEA
		//printf("PRIMERA LINEA\n");
		count += check_position(matrix, x + 1, y);
		count += check_position(matrix, x - 1, y);
		count += check_position(matrix, x + 1, y + 1);
		count += check_position(matrix, x - 1, y + 1);
		count += check_position(matrix, x, y + 1);
	}
	else if (y == d - 1){ // SI ES LA ULTIMA LINEA
		//printf("ULTIMA LINEA\n");
		count += check_position(matrix, x + 1, y);
		count += check_position(matrix, x - 1, y);
		count += check_position(matrix, x + 1, y - 1);
		count += check_position(matrix, x - 1, y - 1);
		count += check_position(matrix, x, y - 1);
	}
	else{ // SI ES EN CUALQUIER OTRA POSICION
		//printf("CENTRO\n");
		count += check_position(matrix, x + 1, y);
		count += check_position(matrix, x - 1, y);
		count += check_position(matrix, x, y + 1);
		count += check_position(matrix, x, y - 1);
		count += check_position(matrix, x + 1, y - 1);
		count += check_position(matrix, x + 1, y + 1);
		count += check_position(matrix, x - 1, y + 1);
		count += check_position(matrix, x - 1, y - 1);
	}
	return count;
}

void born_or_die_cell(Tablero * tablero, Cell *** matrix, int x, int y, int a, int b, int c, int d){
	Cell * cell = matrix[y][x];
	int count = count_neighbour_cells(matrix, x, y, d);
	//printf("(%i, %i) -> COUNT: %i\n", x, y, count);
	if (count == a){
		//printf("NEW CELL\n");
		if (!cell -> viva){ // SI NO ESTA VIVA
			cell -> before = 1;
			tablero -> cell_count ++;
		}
	}
	else if (count < b || count > c){
		//printf("DIE CELL\n");
		if (cell -> viva){
			cell -> before = 0;
			tablero -> cell_count --;
		}
	}
	else if (count >= b && count <= c){
		//printf("SURVIVE CELL\n");
	}
}

void free_tablero(Cell *** current_matrix, int d){
	for (int i = 0; i < d; i++){
		Cell ** current_line = current_matrix[i];
		for (int j = 0; j < d; j++){
			free(current_line[j]);
		}
		free(current_line);
	}
	free(current_matrix);
}

void change_cell_status(Cell *** matrix, int x, int y){
	matrix[y][x] -> viva = matrix[y][x] -> before;
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

		Tablero * tablero = malloc(sizeof(Tablero));
		tablero -> a = a;
		tablero -> b = b;
		tablero -> c = c;
		tablero -> d = d;
		tablero -> execution_time = execution_time;
		tableros[position] = tablero;
		position ++;

		//printf("%s", line);
		char name[MAX_SIZE];
		fscanf(input_file, "%s ", name);
		tablero -> name = malloc(sizeof(name) + 1);
		strcpy(tablero -> name, name);
		int cell_count;
		fscanf(input_file, "%i ", &cell_count);
		tablero -> cell_count = cell_count;
		tablero -> position = 0;

		Cell *** array_matrix = malloc(sizeof(Cell **)*d);
		tablero -> cells_matrix = array_matrix;
		for (int i = 0; i < d; i++){
			Cell ** array_line = malloc(sizeof(Cell *)*d);
			array_matrix[i] = array_line;
			for (int j = 0; j < d; j++){
				array_line[j] = malloc(sizeof(Cell));
				array_line[j] -> x = i;
				array_line[j] -> y = d - j + 1;
				array_line[j] -> viva = 0;
				array_line[j] -> before = 0;
			}
		}

		tablero -> states = malloc(sizeof(Cell ***)*4);
		for (int k = 0; k < 4; k++){
			Cell *** array_state = malloc(sizeof(Cell **)*d);
			tablero -> states[k] = array_state;
			printf("STAGE %i\n", k);
			for (int i = 0; i < d; i++){
				Cell ** state_line = malloc(sizeof(Cell *)*d);
				array_state[i] = state_line;
				for (int j = 0; j < d; j++){
					state_line[j] = malloc(sizeof(Cell));
					state_line[j] -> x = i;
					state_line[j] -> y = d - j + 1;
					state_line[j] -> viva = 0;
					state_line[j] -> before = 0;
					printf("%i ", state_line[j] -> viva);
				}
				printf("\n");
			}
			printf("\n");
		}

		while (cell_count > 0){
			int x, y;
			if (cell_count == 1){
				fscanf(input_file, "%i %i\n", &x, &y);
				Cell * cell = array_matrix[y][x];
				cell -> x = x;
				cell -> y = y;
				cell -> viva = 1;
				cell -> before = 1;
			}
			else{
				fscanf(input_file, "%i %i ", &x, &y);
				Cell * cell = array_matrix[y][x];
				cell -> x = x;
				cell -> y = y;
				cell -> viva = 1;
				cell -> before = 1;
			}

			cell_count --;
		}
		input_count ++;
	}

	print_tablero(tableros[0]);
	//print_tablero(tableros[1]);
	//print_tablero(tableros[2]);


	//######################################
	//##   SIMULACION DE LOS PROCESOS     ##
	//######################################
	Tablero * simulation_tablero = tableros[0];
	Cell *** tablero_cells_matrix = simulation_tablero -> cells_matrix;
	a = simulation_tablero -> a;
	b = simulation_tablero -> b;
	c = simulation_tablero -> c;
	d = simulation_tablero -> d;

	while (simulation_tablero -> execution_time != 0  /*|| simulation_tablero -> cell_count != 0  AGREGAR CONDICIONES DE LOOP Y DE CTRL + C*/){
		printf("EXECUTION TIME: %i CELL COUNT: %i\n", simulation_tablero -> execution_time, simulation_tablero -> cell_count);
		for (int j = 0; j < simulation_tablero -> d; j++){
			for (int i = 0; i < simulation_tablero -> d; i++){
				born_or_die_cell(simulation_tablero, tablero_cells_matrix, i, j, a, b, c, d);
			}
		}
		for (int j = 0; j < simulation_tablero -> d; j++){
			for (int i = 0; i < simulation_tablero -> d; i++){
				change_cell_status(tablero_cells_matrix, i, j);
			}
		}
		print_tablero(simulation_tablero);
		simulation_tablero -> execution_time --;
	}

	//######################################
	//##  LIBERANDO MEMORIA DEL PROGRAMA  ##
	//######################################

	printf("\nLIBERANDO MEMORIA\n");
	for (int tablero_count = 0; tablero_count < table_count; tablero_count ++){
		Tablero * current_tablero = tableros[tablero_count];
		printf("\nFREEING TABLERO %s\n", current_tablero -> name);

		// LIBERANDO MATRIX
		Cell *** matrix = current_tablero -> cells_matrix;
		free_tablero(matrix, current_tablero -> d);

		for (int i = 0; i < 4; i++){
			printf("LIBERANDO MATRIZ ESTADOS %i\n", i);
			free_tablero(current_tablero -> states[i], current_tablero -> d);
		}
		free(current_tablero -> states);

		char * current_name = current_tablero -> name;
		free(current_name);
		free(current_tablero);
	}
	free(tableros);


	fclose(input_file);

	return 0;
}
