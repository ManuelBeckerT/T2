#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 256

struct Cell{
	int viva;
	int before;
	int x;
	int y;
};

typedef struct Cell Cell;

struct Tablero{
	int a;
	int b;
	int c;
	int d;
	int execution_time;

	char * name;
	int cell_count;
	int position;

	Cell *** cells_matrix;

	int state_array_count;
	int state_array_position;
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

void add_state(Tablero * tablero){
	if (tablero -> state_array_position == 4){
		tablero -> state_array_position = 0;
	}
	Cell **** states = tablero -> states;
	for (int j = 0; j < tablero -> d; j++){
		for (int i = 0; i < tablero -> d; i++){
			states[tablero -> state_array_position][j][i] -> viva = tablero -> cells_matrix[j][i] -> viva;
		}
	}
	if (tablero -> state_array_count < 4){
		tablero -> state_array_count++;
	}
	tablero -> state_array_position++;
}

int check_states(Tablero * tablero){
	Cell *** matrix = tablero -> cells_matrix;
	Cell *** state_a = tablero -> states[0];
	Cell *** state_b = tablero -> states[1];
	Cell *** state_c = tablero -> states[2];
	Cell *** state_d = tablero -> states[3];
	int same_a = 0;
	int same_b = 0;
	int same_c = 0;
	int same_d = 0;

	for (int j = 0; j < tablero -> d; j++){
		for (int i = 0; i < tablero -> d; i++){
			if (same_a == 1 && same_b == 1 && same_c == 1 && same_d == 1){
				return 1;
			}
			if (matrix[j][i] -> viva != state_a[j][i] -> viva && same_a == 0){
				same_a = 1;
			}
			if (matrix[j][i] -> viva != state_b[j][i] -> viva && same_b == 0){
				same_b = 1;
			}
			if (matrix[j][i] -> viva != state_c[j][i] -> viva && same_c == 0){
				same_c = 1;
			}
			if (matrix[j][i] -> viva != state_d[j][i] -> viva && same_d == 0){
				same_d = 1;
			}
		}
	}
	return 0;

}

int main(int argc, char** argv)
{
	if (argc > 3 || argc < 3){
		printf("Modo de uso ./life <file> <t>\n");
		return 0;
	}

	FILE* input_file = fopen(argv[1], "r");
	/* Falló la apertura del archivo */
	if(!input_file){
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
		tablero -> state_array_position = 0;
		tablero -> state_array_count = 0;
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
			for (int i = 0; i < d; i++){
				Cell ** state_line = malloc(sizeof(Cell *)*d);
				array_state[i] = state_line;
				for (int j = 0; j < d; j++){
					state_line[j] = malloc(sizeof(Cell));
					state_line[j] -> viva = -1;
				}
			}
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


	//######################################
	//##   SIMULACION DE LOS PROCESOS     ##
	//######################################

	printf("\n\n");
	printf("MAIN PROCESS PID %i\n", getpid());

	int process_count = table_count;
	int value = 0;
	while (process_count != 0){
		if (fork() == 0){
			//printf("HELLO I AM A CHILD - TABLERO %i\n\n", process_count);
			value = process_count - 1;
			printf("PID %i\n", getpid());
		}
		else{
			//printf("I AM A PARENT AND WONT DO NOTHING\n");
			process_count = 1;
			printf("PID %i\n", getpid());
		}
		process_count --;
	}
	printf("%i\n", value);


	//print_tablero(tableros[0]);
	//print_tablero(tableros[value]);
	//print_tablero(tableros[2]);

	Tablero * simulation_tablero = tableros[value];
	Cell *** tablero_cells_matrix = simulation_tablero -> cells_matrix;
	a = simulation_tablero -> a;
	b = simulation_tablero -> b;
	c = simulation_tablero -> c;
	d = simulation_tablero -> d;

	int loop = 0;
	int simulation_time = 0;

	//printf("SIMULATION TIME %i - LOOP %i - CELL COUNT %i\n", simulation_time, loop, simulation_tablero -> cell_count);

	while (simulation_tablero -> execution_time != simulation_time  && simulation_tablero -> cell_count != 0  && loop != 1){
		//printf("EXECUTION TIME: %i CELL COUNT: %i\n", simulation_tablero -> execution_time, simulation_tablero -> cell_count);
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
		if(check_states(simulation_tablero)){
			add_state(simulation_tablero);
		}
		else{
			loop = 1;
			//printf("SAME STATE\n");
		}

		//printf("SIMULATION TIME %i - LOOP %i - CELL COUNT %i\n", simulation_time, loop, simulation_tablero -> cell_count);
		simulation_time ++;

	}
	//print_tablero(simulation_tablero);

	//printf("###################################\n###################################\n###################################\n\n\n");

	//######################################
	//##  LIBERANDO MEMORIA DEL PROGRAMA  ##
	//######################################

	//printf("\nLIBERANDO MEMORIA\n");
	for (int tablero_count = 0; tablero_count < table_count; tablero_count ++){
		Tablero * current_tablero = tableros[tablero_count];
		//printf("\nFREEING TABLERO %s\n", current_tablero -> name);

		// LIBERANDO MATRIX
		Cell *** matrix = current_tablero -> cells_matrix;
		free_tablero(matrix, current_tablero -> d);

		for (int i = 0; i < 4; i++){
			//printf("LIBERANDO MATRIZ ESTADOS %i\n", i);
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
