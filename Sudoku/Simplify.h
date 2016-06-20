#ifndef _SIMPLIFY_H
#define _SIMPLIFY_H

#include <vector>
using namespace std; 

class Simplify
{
	public: 
		bool rowSimplify(vector<vector<bool> >&, vector<vector<char> >&); 
		bool columnSimplify(vector<vector<bool> >&, vector<vector<char> >&);
		void scanBlock(int*, int*); 
		bool blockSimplify(vector<vector<bool> >&, vector<vector<char> >&); 
		void changeCell(int, int, int, int, vector<vector<bool> >, vector<vector<char> >&, bool* changed, bool* changed2);
		void error(vector<vector<char> >, char, int, int, bool*); 
		void hiddenSingles(vector<vector<bool> >& Puzzle, vector<vector<char> >& input);  
};

#endif
