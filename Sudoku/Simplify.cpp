#include "Simplify.h"
#include <iostream>
#include <algorithm> 
#include <stack> 

using namespace std; 

bool Simplify::rowSimplify(vector<vector<bool> >& Puzzle, vector<vector<char> >& input)
{
	int row = 0; 
	int num = 0;
 	int j = 0; 
	bool changed = false; 
	bool changed2 = false; 
	bool error1 = false; 
	bool errorRec1 = false, errorRec2 = false, errorRec3 = false;  

	for (int i = 0; i < 81; i++) 
	{
		if(i > 8)
		{ 
			if ((i%9) == 0)
			{
				row++; 
				j = 0; 
			}
		}

		//If the cell already decided skip it
		if(input[row][j] != '.')
		{
			j++; 
			continue; 
		}


		//Scan the row for each cell
		for (int column = 0; column < 9; column++) 
		{ 
			if(input[row][column] == '.') 
			{
				continue; 
			}

			else 
			{
				num = (input[row][column] - '0') - 1;
				Puzzle[i][num] = 0; 
			}

		}

		//Possiblities narrowed to 1 change that cell to the appropriate
		//number
        if (input[row][j] == '.')
			changeCell(i, 0, row, j, Puzzle, input, &changed, &changed2); 

		if (changed2 == true)
			error(input, input[row][j], row, j, &error1); 

		if (error1 == true) 
			return error1; 			

		j++;

		error1 = false;
		changed2 = false;  

	}
	
	//If we decided a cell do rowSimplify again
	if (changed == true)
	{
		errorRec1 = rowSimplify(Puzzle, input); 
		errorRec2 = columnSimplify(Puzzle, input); 
		errorRec3 = blockSimplify(Puzzle, input); 
	}

	if (errorRec1 == true || errorRec2 == true || errorRec3 == true)
		return true; 
	
	return false; 
}

bool Simplify::columnSimplify(vector<vector<bool> >& Puzzle, vector<vector<char> >& input)        
{
    int row = 0;
    int num = 0;
    int j = 0;
    bool changed = false;
	bool changed2 = false;
	bool error1 = false;  
    bool errorRec1 = false, errorRec2 = false, errorRec3 = false;



    for (int i = 0; i < 81; i++)
    {
        if(i > 8)
        {
            if ((i%9) == 0)
            {
                row++;
                j = 0;
            }
        }


        if(input[row][j] != '.')
        {
            j++;
            continue;
        }


		//We iterated through the column instead of the row
        for (int r = 0; r < 9; r++)
        {
            if(input[r][j] == '.')
            {
                continue;
            }

            else
            {
                num = (input[r][j] - '0') - 1;
                Puzzle[i][num] = 0;
            }

        }
		//


        if (input[row][j] == '.')
			changeCell(i, 0, row, j, Puzzle, input, &changed, &changed2); 


        if (changed2 == true)
            error(input, input[row][j], row, j, &error1);

        if (error1 == true)
            return error1;

        j++;

        error1 = false;
        changed2 = false;

    }


    if (changed == true)
	{
        errorRec1 = columnSimplify(Puzzle, input);
		errorRec2 = rowSimplify(Puzzle, input); 
		errorRec3 = blockSimplify(Puzzle, input); 
	}


    if (errorRec1 == true || errorRec2 == true || errorRec3 == true)
        return true;


	return false; 
}



bool Simplify::blockSimplify(vector<vector<bool> >& Puzzle, vector<vector<char> >& input)
{
    int row = 0;
    int num = 0;
    int j = 0;
	int scanR; 
	int scanC;
	int limitR; 
	int limitC;  
    bool changed = false;
	bool changed2 = false; 
	bool error1 = false; 
    bool errorRec1 = false, errorRec2 = false, errorRec3 = false;



    for (int i = 0; i < 81; i++)
    {
        if(i > 8)
        {
            if ((i%9) == 0)
            {
                row++;
                j = 0;
            }
        }


        if(input[row][j] != '.')
        {
            j++;
            continue;
        }

		scanR = row; 
		scanC = j; 

		scanBlock(&scanR, &scanC);
		int save = scanC; 

	    limitR = scanR + 3;
        limitC = scanC + 3;


   		for (; scanR < limitR; scanR++)
        {
			for (; scanC < limitC; scanC++) 
			{
            	if(input[scanR][scanC] == '.')
            	{
                	continue;
            	}

            	else
            	{
                	num = (input[scanR][scanC] - '0') - 1;
                	Puzzle[i][num] = 0;
           	 	}

        	}
			scanC = save; 
		}	



        if (input[row][j] == '.')
			changeCell(i, 0, row, j, Puzzle, input, &changed, &changed2); 		

        if (changed2 == true)
            error(input, input[row][j], row, j, &error1);

        if (error1 == true)
            return error1;

        j++;

        error1 = false;
        changed2 = false;

    }


    if (changed == true)
    {
        errorRec1 = columnSimplify(Puzzle, input);
        errorRec2 = rowSimplify(Puzzle, input);
		errorRec3 = blockSimplify(Puzzle, input); 
    }

    if (errorRec1 == true || errorRec2 == true || errorRec3 == true)
        return true;

	return false; 
}

//Find the appropriate block to scan for the cell
void Simplify::scanBlock(int* row, int* column)
{

	if ((0 <= *row && *row < 3)	&& (0 <= *column && *column < 3))
	{
		*row = 0; 
		*column = 0; 
	}
	
	else if ((0 <= *row && *row < 3) && (2 < *column && *column < 6))
	{
		*row = 0; 
		*column = 3; 
	}

	else if ((0 <= *row && *row < 3) && (5 < *column && *column < 9))
	{
		*row = 0; 
		*column = 6; 
	}

	else if ((2 < *row && *row < 6) && (0 <= *column && *column < 3))
	{
		*row = 3; 
		*column = 0; 
	}

	else if ((2 < *row && *row < 6) && (2 < *column && *column < 6))
	{
		*row = 3; 
		*column = 3; 
	}

	else if ((2 < *row && *row < 6) && (5 < *column && *column < 9))
	{
		*row = 3; 
		*column = 6; 
	}
	
	else if((5< *row && *row < 9) && (0 <= *column && *column < 3))
	{
		*row = 6; 
		*column = 0; 
	}

	else if ((5 < *row && *row < 9) && (2 < *column && *column < 6)) 
	{
		*row = 6; 
		*column = 3; 
	}
		
	else if ((5 < *row && *row < 9) && (5 < *column && *column< 9)) 
	{
		*row = 6; 
		*column = 6; 
	}


}

void Simplify::changeCell(int index, int count, int row, int column,
vector<vector<bool> > Puzzle, vector<vector<char> >& input, bool* changed, bool* changed2)
{

	//Value of the cell in the char array 
	int pos = 0; 
	int value = 0; 

	for (int a = 0; a < 9; a++)
    {
    	if(Puzzle[index][a] == true)
        {
        	count++;
           	pos = a + 1;
			value = pos + '0'; 
        }
   	}

 	if (count == 1)
    {
    	*changed = true;
		*changed2 = true; 
        input[row][column] = value;
    }

}

		
void Simplify::error(vector<vector<char> > input, char value, int row, int column
,bool* error1)
{
	int scanR = row;
    int scanC = column;

    int save = scanC;

    int limitR;
    int limitC;

	//Check for duplicate values
	//Scan the row
    for (int i = 0; i < 9; i++)
	{
		if(i == column)
			continue; 			

        if(input[row][i] == value)
		{
			*error1 = true; 
			break; 
		}
	}

	//Scan the column
    for (int i = 0; i < 9; i++)
	{
		if(i == row)
			continue; 		

        if(input[i][column] == value)
		{
			*error1 = true; 
			break; 
		}

	}

	scanBlock(&scanR, &scanC); 

	limitR = scanR + 3; 
	limitC = scanC + 3; 

	for(; scanR < limitR; scanR++) 
	{
		for(; scanC < limitC; scanC++) 
		{
			if(scanR == row && scanC == column)
				continue; 

			if (input[scanR][scanC] == value)
			{
				*error1 = true; 
				break; 
			}
		}

		scanC = save; 
	}


}





