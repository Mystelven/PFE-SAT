/**************************************************************************************************
Copyright (c) 2014, SATyr - Valentin Montmirail - Polytech Tours, France

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Solver_H
#define Solver_H

/** A Solver is just a value for each variables */
#include "Problem.h"
using namespace std;

/**
 * A Solver is just a value for each Variable of every Clause in one Problem.
 * @author Valentin.M
 * @version 1.0
 * @see Variable
 */
class Solver {

public:

	Solver(Problem p);

	inline int getTheSolution(unsigned int i);

	inline bool isOpen(Clause c);
	
	inline size_t getSize();

	bool isSolution(Problem p);
	
	void solve(Problem p);
	
	inline void inverse(unsigned int i);

	/** to display a Strategie */
	void display(ostream &out) const;


private:

	int* arrayOfSolutions;
	size_t size;


};

	/** We redefined the output to display a clause during debugging phases. */
	std::ostream& operator<<(std::ostream& out, Solver const& Solver);



#endif
