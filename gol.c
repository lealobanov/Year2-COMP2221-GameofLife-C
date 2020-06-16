#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "gol.h"

void read_in_file(FILE *infile, struct universe *u) {
int temp_cols;
char c;
int original_alive = 0; 
int max_cols = -1;
(*u).rows=0;
(*u).cols=0;
if ((*u).rows == 0){
            //Malloc
             (*u).grid = malloc(1 * sizeof(char *)); // Allocate row pointers
            if ((*u).grid == NULL){
            fprintf(stderr, "Error executing malloc.\n");
            exit(1);
            }
           ((*u).grid[0] )= malloc(512 * sizeof(char));
            if ((*u).grid[0] == NULL){
                fprintf(stderr, "Error executing malloc.\n");
                exit(1);
                }      
}
while( (c = fgetc(infile)) && EOF != c) {
	
if (((*u).rows > 0) && ((*u).cols==0)){
 //Realloc
 (*u).tmpgrid= realloc((*u).grid, sizeof(char *)*((*u).rows+1));
            if( (*u).tmpgrid == NULL){
            fprintf(stderr, "Error executing realloc.\n");
            exit(1);
            }
            (*u).grid = (*u).tmpgrid;
            (*u).grid[(*u).rows] = malloc(512 * sizeof(char));
            if ((*u).grid[(*u).rows] == NULL){
                fprintf(stderr, "Error executing malloc to new row.\n");
                exit(1);
                }
}

if (c != '\n'){
    if (c == '*'){
        original_alive++;
    }
     if (c != '*' && c != '.') {
             //If chars other than * and . in the matrix, reject and throw an error
                fprintf(stderr, "Contents of supplied input file must be * or . ;this includes redundant white space and new line characters.\n");
                exit(1);
    }
    (*u).grid[(*u).rows][(*u).cols]=c;
   ((*u).cols)++;
   //If number of columns exceeds 512, throw and error
   if ((*u).cols >=513){
       fprintf(stderr, "Invalid file; number of columns exceeded.\n");
        exit(1);
   }
}

if (c == '\n'){
    //Check if # of columns differs throughout rows of input grid
    if ((*u).rows == 0){
        max_cols = (*u).cols;
    } else if (max_cols != (*u).cols){
    fprintf(stderr, "Misaligned columns.\n");
        exit(1);
    } 
((*u).rows)++;
temp_cols = (*u).cols;
(*u).cols= 0;
}

}
(*u).cols = temp_cols;
//printf("final cols: %d\n",(*u).cols);
//printf("final rows %d\n",(*u).rows);

//Check for rows or colums = 0 in final input
if ((*u).cols == 0 || (*u).rows == 0){
     fprintf(stderr, "Empty rows or columns.\n");
        exit(1);
}
//Initalize statistics metrics
(*u).current_alive = original_alive;
(*u).total_alive =  original_alive;
(*u).gen = 1;
}


void write_out_file(FILE *outfile, struct universe *u){
	int row_dims = u->rows;
int col_dims = u->cols;
int i;
int j;
//Iterate through grid row by row, printing to the specified destination file
for(i = 0; i < row_dims;i++){
    for(j = 0; j <= col_dims; j++){
        if (j == col_dims){
            //Reached end of row, add new line character
            fprintf(outfile, "\n");
        } else {
            //Else print character at that position
            fprintf(outfile, "%c",u->grid[i][j]);  
        }  
    }
}
//Free previously allocated memory in universe grid
for(int i = 0; i < row_dims; i++)
    free(u->grid[i]);
free(u->grid);
}

int is_alive(struct universe *u, int column, int row){
    //returns 1 if the cell in that column and row is alive and 0 otherwise
    if (u->grid[row][column] == '*') {
        return 1;
    } else {
        return 0;
    }
}  

int will_be_alive(struct universe *u, int column, int row){
	int alive_neighbors = 0;
    int row_boundary = u->rows;
    int col_boundary = u->cols;
    //Checking the cell's neighbors
    //Check right neighbor
    if ((column+1)<col_boundary){
        if(u->grid[row][column+1] == '*'){
        alive_neighbors++;
        }
    }
    // Check left neighbor
    if ((column-1)>=0){
        if(u->grid[row][column-1] == '*'){
        alive_neighbors++;
        }
    }
    // Check top neighbor
    if ((row-1)>=0){
        if(u->grid[row-1][column] == '*'){
        alive_neighbors++;
        }
    }
    // Check bottom neighbor
    if ((row+1)<row_boundary){
        if(u->grid[row+1][column] == '*'){
        alive_neighbors++;
        }
    }
    // Check top-right-diagonal neighbor
    if ((row-1)>=0 && (column+1)<col_boundary){
        if(u->grid[row-1][column+1] == '*'){
        alive_neighbors++;
        }
    }
    // Check top-left-diagonal neighbor
    if ((row-1)>=0 && (column-1)>=0){
        if(u->grid[row-1][column-1] == '*'){
        alive_neighbors++;
        }
    }
    // Check bottom-right-diagonal neighbor
    if ((row+1)<row_boundary && (column+1)<col_boundary){
        if(u->grid[row+1][column+1] == '*'){
        alive_neighbors++;
        }
    }
    // Check bottom-left-diagonal neighbor
    if ((row+1)<row_boundary && (column-1)>=0){
        if(u->grid[row+1][column-1] == '*'){
        alive_neighbors++;
        }
    }
    // Total number of alive neighbors now stored in alive_neighbors
    if (is_alive(u,column,row) == 1) {
        // The cell is alive in the current generation
        if (alive_neighbors == 2 || alive_neighbors == 3){
            return 1;
        } else {
            return 0;
        }
    } else {
        // The cell is dead in the current generation
        if (alive_neighbors == 3) {
            return 1;
        }
        else {
            return 0;
        }
    }
}


int will_be_alive_torus(struct universe *u,  int column, int row){
int alive_neighbors = 0;
    int row_boundary = u->rows;
    int col_boundary = u->cols;
    //Checking the cell's neighbors
     // Check right neighbor
    if ((column+1)<col_boundary){
        if(u->grid[row][column+1] == '*'){
        alive_neighbors++;
        }
    } else {
        if(u->grid[row][0] == '*'){
        alive_neighbors++;
        }
    }
    // Check left neighbor
    if ((column-1)>=0){
        if(u->grid[row][column-1] == '*'){
        alive_neighbors++;
        }
    } else {
        if(u->grid[row][col_boundary-1] == '*'){
        alive_neighbors++;
        }
    }
    // Check top neighbor
    if ((row-1)>=0){
        if(u->grid[row-1][column] == '*'){
        alive_neighbors++;
        }
    } else {
        if(u->grid[row_boundary-1][column] == '*'){
        alive_neighbors++;
        }
    }
    // Check bottom neighbor
    if ((row+1)<row_boundary){
        if(u->grid[row+1][column] == '*'){
        alive_neighbors++;
        }
    } else {
        if(u->grid[0][column] == '*'){
        alive_neighbors++;
        }
    }
    // Check top-right-diagonal neighbor
    if ((row-1)>=0 && (column+1)<col_boundary){
        if(u->grid[row-1][column+1] == '*'){
        alive_neighbors++;
        }
    } else {
        //Check if row, column, or both is out of bounds
        if ((row-1)<0 && (column+1)<col_boundary){
        //Row out of bounds
            if(u->grid[row_boundary-1][column+1] == '*'){
            alive_neighbors++;
            }
        } else if((row-1)>=0 && (column+1)>=col_boundary){
        //Column out of bounds
            if(u->grid[row-1][0] == '*'){
            alive_neighbors++;
            }
        } else {
        //Both row/col out of bounds; corner case  
            if(u->grid[row_boundary-1][0] == '*'){
            alive_neighbors++;
            }  
        }
    }
    // Check top-left-diagonal neighbor
    if ((row-1)>=0 && (column-1)>=0){
        if(u->grid[row-1][column-1] == '*'){
        alive_neighbors++;
        }
    } else {
        //Check if row, column, or both is out of bounds
        if ((row-1)<0 &&(column-1)>=0){
        //Row out of bounds
            if(u->grid[row_boundary-1][column-1] == '*'){
            alive_neighbors++;
            }
        } else if((row-1)>=0 &&(column-1)<0){
        //Column out of bounds
            if(u->grid[row-1][col_boundary-1] == '*'){
            alive_neighbors++;
            }
        } else {
        //Both row/col out of bounds; corner case  
            if(u->grid[row_boundary-1][col_boundary-1] == '*'){
            alive_neighbors++;
            }  
        }
    }
    // Check bottom-right-diagonal neighbor
    if ((row+1)<row_boundary && (column+1)<col_boundary){
        if(u->grid[row+1][column+1] == '*'){
        alive_neighbors++;
        }
    } else {
        //Check if row, column, or both is out of bounds
        if ((row+1)>=row_boundary &&(column+1)<col_boundary){
        //Row out of bounds
            if(u->grid[0][column+1] == '*'){
            alive_neighbors++;
            }
        } else if((row+1)<row_boundary &&(column+1)>=col_boundary){
        //Column out of bounds
            if(u->grid[row+1][0] == '*'){
            alive_neighbors++;
            }
        } else {
        //Both row/col out of bounds; corner case  
            if(u->grid[0][0] == '*'){
            alive_neighbors++;
            }  
        }
    }
    // Check bottom-left-diagonal neighbor
    if ((row+1)<row_boundary && (column-1)>=0){
        if(u->grid[row+1][column-1] == '*'){
        alive_neighbors++;
        }
    } else {
         //Check if row, column, or both is out of bounds
        if ((row+1)>=row_boundary &&(column-1)>=0){
        //Row out of bounds
            if(u->grid[0][column-1] == '*'){
            alive_neighbors++;
            }
        } else if((row+1)<row_boundary &&(column-1)<0){
        //Column out of bounds
            if(u->grid[row+1][col_boundary-1] == '*'){
            alive_neighbors++;
            }
        } else {
        //Both row/col out of bounds; corner case  
            if(u->grid[0][col_boundary-1] == '*'){
            alive_neighbors++;
            }  
        }
    }
    // Total number of alive neighbors now stored in alive_neighbors
    if (is_alive(u,column,row) == 1) {
        // The cell is alive in the current generation
        if (alive_neighbors == 2 || alive_neighbors == 3){
            return 1;
        } else {
            return 0;
        }
    } else {
        // The cell is dead in the current generation
        if (alive_neighbors == 3) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)){
//Malloc another universe of the same size for temp calculations
int row_dim = u->rows;
int col_dim = u->cols;
 (*u).temp = malloc(row_dim * sizeof(char *)); // Allocate row pointers
if ((*u).temp == NULL){
        fprintf(stderr, "Error executing malloc.\n");
        exit(1);
        }    
int i; 
for(i = 0; i < row_dim; i++) {
    (*u).temp[i] = malloc(col_dim * sizeof(char));
    if ((*u).temp[i] == NULL){
        fprintf(stderr, "Error executing malloc.\n");
        exit(1);
        } 
}
 
//Iterate through current grid
int j;
int alive_in_gen = 0; 
for(i = 0; i < row_dim; i++) {
    for(j = 0; j < col_dim; j++) {
        if ((*rule)(u,j,i) == 1) {
        //Cell at position [i][j] will be alive in the next generation
            (*u).temp[i][j] = '*';
            alive_in_gen += 1;
        } else {
        //Cell at position [i][j] will be dead in the next generation
            (*u).temp[i][j] = '.';
        }
    }
}  
//Migrate new generation stored in temp to u->grid
for(i = 0; i < row_dim; i++) {
    for(j = 0; j < col_dim; j++) {
       (*u).grid[i][j] = (*u).temp[i][j];   
    }
}  
//Free previously allocated memory in temp
for(int i = 0; i < row_dim; i++)
    free((*u).temp[i]);
free((*u).temp);

//Update statistic metrics
(*u).current_alive  = alive_in_gen;
(*u).total_alive += alive_in_gen;
(*u).gen++;
}

void print_statistics(struct universe *u){

//printf("received current %d", current_alive);
float total_alive = u->total_alive;
	float current_alive = u->current_alive;
float row_cells = u->rows;
float col_cells = u->cols;
float total_cells = row_cells * col_cells;
int gen = (*u).gen;
float temp = total_alive/gen;

//Evaluate percentage with precision to 3 decimal places
float percent_current = (current_alive/total_cells) * 100;
float percent_avg = ((temp/total_cells)* 100);
printf("%0.3f%% of cells currently alive\n", percent_current);
printf("%0.3f%% of cells alive on average\n", percent_avg);
}
