#ifndef _GUESS_H
#define _GUESS_H

#include <vector>
#include <stack>
#include "Puzzle.h"

using namespace std;

class Guess
{
	private:
		stack<Puzzle> alternatives;
		stack<vector<vector<char> > > array;  

	public:
		Guess(Puzzle, vector<vector<char> >); 
		void choose(); 
};

#endif

