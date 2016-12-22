#include <stdio.h>
#include <time.h>

int column = 25;
int row = 50;

int display_Grid(int rows, int columns, char grid[rows][columns]);

int main () 
{
	char grid[row][column];

	display_Grid(row, column, grid);

	return 0;
}

int display_Grid(int rows, int columns, char grid[rows][columns]) 
{
	for(int i = 0; i <= rows; i++)
	{	
		for(int j = 0; j <= columns; j++)
		{
			grid[j][i] = '_';
			printf("_");
		}
		printf("\n");
	}

	return 0;
}
