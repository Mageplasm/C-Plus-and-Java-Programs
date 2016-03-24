#include "Puzzle.h"

//Returns the grid containing options for each element
vector<vector<bool> > Puzzle::getPuzzle() const 
{
	return grid_options; 
}

void Puzzle::setPuzzle(vector<vector<bool> > options)
{
	this->grid_options = options; 
}
