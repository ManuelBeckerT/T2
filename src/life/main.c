#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include  <signal.h>

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

	int end; // 0 LOOP, 1 NOCELLS, 2 NOTIME, 3 SIGNAL
	int simulation_time;
};

typedef struct Tablero Tablero;

#define SIGINT  2

void print_tablero(Tablero * tablero){
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

void cell_life_simulation(Tablero * simulation_tablero, int table_count, Tablero ** tableros){
	printf("\n%s INICIAL\n", simulation_tablero -> name);
	print_tablero(simulation_tablero);
	Cell *** tablero_cells_matrix = simulation_tablero -> cells_matrix;
	int a = simulation_tablero -> a;
	int b = simulation_tablero -> b;
	int c = simulation_tablero -> c;
	int d = simulation_tablero -> d;

	int loop = 0;
	int simulation_time = 0;

	while (simulation_tablero -> execution_time != simulation_time  && simulation_tablero -> cell_count != 0  && loop != 1){
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
		}

		simulation_time ++;

	}
	printf("%s FINAL\n", simulation_tablero -> name);
	print_tablero(simulation_tablero);
	if (simulation_time == simulation_tablero -> execution_time){
		printf("%s Término por tiempo. Tiempo simulación %i\n", simulation_tablero -> name, simulation_time);
		simulation_tablero -> end = 2;
	}
	else if (simulation_tablero -> cell_count == 0){
		printf("%s Término por falta de células. Tiempo simulación %i\n", simulation_tablero -> name, simulation_time);
		simulation_tablero -> end = 1;
	}
	else if (loop == 1){
		printf("%s Término por loop. Tiempo simulación %i\n", simulation_tablero -> name, simulation_time);
		simulation_tablero -> end = 0;
	}
	else{
		printf("%s Término por interrupción. Tiempo simulación %i\n", simulation_tablero -> name, simulation_time);
		simulation_tablero -> end = 3;
	}
	simulation_tablero -> simulation_time = simulation_time;
	_exit(0);
}

void  INThandler(int sig){

     char  c;

     signal(sig, SIG_IGN);
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
          _exit(0);

     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
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

	int table_count, a, b, c, d;
	fscanf(input_file, "%i %i  %i %i %i", &table_count, &a, &b, &c, &d);

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
		tablero -> end = 3;
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

	signal(SIGINT, INThandler);

	//######################################
	//##   SIMULACION DE LOS PROCESOS     ##
	//######################################

	int process_count = table_count;
	int pid = 0;
	for (int p = 0; p < process_count; p++){
		if (pid == vfork()){
			cell_life_simulation(tableros[p], table_count, tableros);
		}
		else{
			wait(NULL);
		}
	}

	//######################################
	//##    ESCRIBIENDO resutlado.csv     ##
	//######################################

	// 0 LOOP, 1 NOCELLS, 2 NOTIME, 3 SIGNAL
	// nombre_tablero_i,tiempo_final_i,cantidad_celulas_i,razon_termino_i
	FILE* output_file = fopen("resultado.csv", "w");
	printf("TERMINAMOS\n");
	for (int i = 0; i < table_count; i++){
		if (tableros[i] -> end == 0){
			fprintf(output_file, "%s,%i,%i,LOOP\n", tableros[i] -> name, tableros[i] -> simulation_time, tableros[i] -> cell_count);
		}
		else if (tableros[i] -> end == 1){
			fprintf(output_file, "%s,%i,%i,NOCELLS\n", tableros[i] -> name, tableros[i] -> simulation_time, tableros[i] -> cell_count);
		}
		else if (tableros[i] -> end == 2){
			fprintf(output_file, "%s,%i,%i,NOTIME\n", tableros[i] -> name, tableros[i] -> simulation_time, tableros[i] -> cell_count);
		}
		else{
			fprintf(output_file, "%s,%i,%i,SIGNAL\n", tableros[i] -> name, tableros[i] -> simulation_time, tableros[i] -> cell_count);
		}
	}

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
	fclose(output_file);


	return 0;
}
