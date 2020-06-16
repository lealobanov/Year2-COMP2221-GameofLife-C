#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gol.h"

int main(int argc, char *argv[]){
 	int	i;
  //Initialize statistics and torus to false
  int stats = 0;
  int torus = 0;
  int out = 0;
  //Standard OS max file name length is 255 characters
  char input_file[255];
  char out_file[255];
  //If user does not supply an alternate value, default number of generations is set to 5
  int gen = 5;
  //Check for redundant switch flags
  int i_count = 0;
  int o_count = 0;
  int g_count = 0;
  int s_count = 0;
  int t_count = 0;

//Parse command line arguments using a switch statement
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case 'i' :
        if (argv[i+1] == NULL){
        fprintf(stderr, "Input file string is empty.\n");
        exit(1);
        }
        else if (i_count > 0){
          if (strcmp(argv[i+1],input_file)){
            fprintf(stderr, "Conflicting command line switches.\n");
          exit(1);
          }
        }
        else { strcpy(input_file, argv[i+1]);
        i_count++;
        }
				i++;
				break;
			case 'o' :
        if (argv[i+1] == NULL){
        fprintf(stderr, "Output file string is empty.\n");
        exit(1);
        }
        else if (o_count > 0){
          if (strcmp(argv[i+1],out_file)){
            fprintf(stderr, "Conflicting command line switches.\n");
          exit(1);
          }
        }
        else {
        strcpy(out_file, argv[i+1]);
        o_count++;
        out=1;
        }
        i++;
				break;
			case 'g' :
        if (argv[i+1] == NULL){
        fprintf(stderr, "Supplied generations is empty.\n");
        exit(1);
        }
        else if (g_count > 0){
          if (atoi(argv[i+1]) != gen){
            fprintf(stderr, "Conflicting command line switches.\n");
          exit(1);
          }
        }
        else if (strcmp(argv[i+1],"0")==0){
          gen = 0;
          g_count++;
        }
        else {
          gen = atoi(argv[i+1]);
          g_count++;
          if (gen == 0){
            fprintf(stderr, "Supplied generations is invalid.\n");
            exit(1);
          }
        }
        i++;
				break;
			case 's' :
				stats = 1;
				break;
      case 't' :
				torus = 1;
				break;
			default :
				fprintf(stderr, "Unknown command line switch provided: %s\n",argv[i]);
        exit(1);
        break;
			}
		} else {
      fprintf(stderr, "Unknown command line switch provided: %s\n",argv[i]);
        exit(1);
    }
	}
int user_input = 0;
//If user did not supply an input file, default to stdin
  if(strlen(input_file) == 0 ){
    user_input = 1;
    }

FILE *infile;
struct universe v; 
if (user_input == 1) {
  read_in_file(stdin, &v);
}
else {
	infile = fopen(input_file, "r");    // Opens file for reading in input
        if (infile == NULL){ //Check if file exists in current directory
        fprintf(stderr, "Could not open specified input file.\n");
        exit(1);
        }
        if (!infile)       {// Checks to see if file has any data
        fprintf(stderr, "Supplied input file is empty.\n"); 
        exit(1);}
  read_in_file(infile, &v);
  //Close file upon reading in its input
  fclose(infile);
}
// Loop through specified number of generations with appropriate rule
int z = 0;
while (z < gen) {
  //Check if using torus rule
  if (torus == 1){
  evolve(&v,will_be_alive_torus);
  } else {
  //Evolve using normal rule  
  evolve(&v,will_be_alive);
  }
  z++ ;
}
 //If statistics requested, calculate appropriate metrics and print to screen after final generation
if (stats == 1){
  print_statistics(&v);
} 
//If user did not supply an output file, print last generation to screen
if(out==0){
	write_out_file(stdout,&v);
  } else if (out==1){
  FILE *outfile;
  outfile = fopen(out_file, "w"); // Opens file for writing out 
  write_out_file(outfile,&v);
  //Close file upon writing out appropriate output
  fclose(outfile);
}
  }
 




 
