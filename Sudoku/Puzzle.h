#ifndef _PUZZLE_H
#define _PUZZLE_H

#include <vector>
using namespace std; 

class Puzzle 
{
	private:
		vector<bool> element_options;
		vector<vector<bool> > grid_options; 

	public: 
		Puzzle() :  element_options(9, 0),  grid_options(81, element_options) {}
		vector<vector<bool> > getPuzzle() const; 
		void setPuzzle(vector<vector<bool> >);  
};

#endif 
