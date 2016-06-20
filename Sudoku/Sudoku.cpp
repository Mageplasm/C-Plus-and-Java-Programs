#include <iostream>
#include <cstdio>
#include <string>
#include "Puzzle.h"
#include "Simplify.h"
#include "Guess.h" 

using namespace std; 

void readPuzzle(vector<vector<char> >&); 
vector<vector<bool> > initOptions(Puzzle, vector<vector<char> >); 

int main()
{
	vector<vector<char> > puzzle(9, vector<char> (9, 0)); 
	Puzzle new_puzz;
	
	readPuzzle(puzzle); 

	vector<vector<bool> > options = initOptions(new_puzz, puzzle);
	
	Simplify sim; 
		
	sim.rowSimplify(options, puzzle); 	
	sim.columnSimplify(options, puzzle); 
	sim.blockSimplify(options, puzzle);

	new_puzz.setPuzzle(options);

	Guess itsG(new_puzz, puzzle);
	
	itsG.choose();  

	vector<vector<bool> > test = new_puzz.getPuzzle(); 
		 	
	return 0; 
}

//Function to read in Puzzle to be solved
void readPuzzle(vector<vector<char> >& Puzzle)
{
	//Initialize row and column to start inputting valeus
	int row = 0;
	int column = 0;

	//Each input element of Sudoku puzzle
	char c;  
	
	while (!feof(stdin) && cin.get(c))
	{		
		if (row > 8)
			continue; 
 
		Puzzle[row][column] = c; 	
		column++;

		if(column == 9)
		{
			column = 0; 
			row++; 
		}
	}
}

//Function to initialize possible options for all 81 elements in the table
vector<vector<bool> > initOptions(Puzzle puzzle, vector<vector<char> > inputPuzz)
{
	int row = 0; 
	int column = 0;

	vector<vector<bool> > options = puzzle.getPuzzle(); 

	//Each element in the table contains 9 possible options 
	//1 indicated the number is possible while 0 indicates it is not
	for(int i = 0; i < 81; i++) 
	{
		if(column == 9) 
		{
			column = 0; 
			row++; 		
		}

		if (inputPuzz[row][column] == '.') 
		{
			for (int j = 0; j < 9; j++) 
			{
				options[i][j] = 1; 
			}
		}	

		else
			options[i][(inputPuzz[row][column] - '0') - 1] = 1;

		column++;  
	}

	return options; 
}


