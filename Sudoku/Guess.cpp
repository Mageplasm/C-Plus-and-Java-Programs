#include "Guess.h"
#include "Simplify.h"
#include <iostream> 

using namespace std; 

Guess::Guess(Puzzle puzz, vector<vector<char> > input) 
{
	alternatives.push(puzz); 
	array.push(input); 
}

void Guess::choose()
{
	//Var to save position of first possible option in the cell
	int save = 0;
	//
	
	int start_pos = 0;  

	//Row and column indexes
	int row = 0; 
	int column = 0;
	// 

	Simplify sim;
	bool exit = false; 

	//Var to let us know when we need to backtrack
	bool backtrack = false;  

	//Errors returned from row, column, and block simplify
	bool guess1 = false, guess2 = false, guess3 = false; 

	//Vectors to store the puzzle 
	vector<vector<char> > input, input2;  
	vector<vector<bool> > options, options2; 

    while (!alternatives.empty())
    {
		//Pop the boolean puzzle from the stack
    	Puzzle puzz = alternatives.top();
		alternatives.pop();

		//Pop the array of puzzle values from the stack	
		input = array.top();
		input2 = array.top(); 
		array.pop();
		//

		options = puzz.getPuzzle(); 
		options2 = puzz.getPuzzle();

        for (int i = 0; i < 81; i++)
        {
            if(column == 9)
            {
                column = 0;
                row++;
            }

			//Guess for the empty cell
            if (input[row][column] == '.')
            {
                for (int j = 0; j < 9; j++)
                {
                    if(options[i][j] == true)
					{
						//Save the position of the first value 
						//that is true
						save = j;
						backtrack = false;  
						break; 
					}

					//If no possibilities work for the cell
					//set backtrack = true
					backtrack = true; 
                }

				//Exit the loop so we can backtrack to an
				//alternative solution
				if (backtrack == true)
				{
					row = 0; 
					column = 0;  
					break; 
				}
			
				start_pos = save + 1;

				//Set all values except the guess to false
				for (; start_pos < 9; start_pos++) 
					options[i][start_pos] = false; 

				//In the second vector set the same value to false
				//Becomes the second alternative
				options2[i][save] = false;

   				Puzzle op1;
    	    	Puzzle op2;
				op2.setPuzzle(options2);

				//Push their corresponding arrays as well 
				input2[row][column] = (save + 1) + '0';

				//Check if guess is okay for now
				guess1 = sim.rowSimplify(options, input2); 
				guess2 = sim.columnSimplify(options, input2); 
				guess3 = sim.blockSimplify(options, input2);

				op1.setPuzzle(options);

				//If guess doesn't work push the alternative
				if (guess1 == true || guess2 == true || guess3 == true)
				{
					alternatives.push(op2); 
					array.push(input);  
				}

				//Else push the alternative and the guess
				else
				{
					alternatives.push(op2);
					alternatives.push(op1);
					array.push(input);  
					array.push(input2);
				} 
					
				
				//Reset column and row 
				column = 0; 
				row = 0; 
 
				break; 
							
            }

			//If all the values in the puzzle are numbers: exit = true
			if (i == 80) 
				exit = true; 

			column++;  
        }
		
		backtrack = false; 

		//Exit the outer while loop
		if (exit == true) 
			break;		
	}

	for (int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++) 
		{
			cout<<input[i][j]<<" "; 
		}
		cout<<endl;
	}
}
