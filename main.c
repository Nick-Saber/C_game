#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ncols 50
#define nrows 25

int display_Grid(char ** grid);

int main () 
{
	//create 2d ncols x nrows grid array and fill it with empty space characters
	char ** grid = malloc(sizeof(char *)*nrows);
	for ( int i = 0; i<nrows; i++){
		grid[i]= (char*) malloc(ncols);
		memset(grid[i],'@',ncols);
	}
	display_Grid(grid);


	return 0;
}

int display_Grid(char ** grid) 
{
	for(int i = 0; i < nrows; i++)
	{	
		for(int j = 0; j < ncols; j++)
		{
			printf("%c",grid[i][j]);
		}
		printf("\n");
	}

	return 0;
}
