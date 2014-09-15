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

	/** 
 	 * This is the constructor of a Solver adapted to a problem.
 	 * @param p the Problem we need to solve.
	 */
	Solver(Problem p);

	/**
	 * This is the copy-constructor of a Solver.
	 * It's needed because there's a pointer in the attribute of the class.
	 * @param s the Original Solver that we will copy.
	 */
	Solver(Solver& s);

	/**
 	 * Check if one clause is open with our current solution
 	 * @param c the Clause we need to check
 	 * @return true if the clause is open, false otherwise.
	 */
	inline bool isOpen(Clause c) const;
	
	/**
	 * This method allows us to get the number of variable in the Problem.
	 */
	inline size_t getSize() const;

	/**
	 * Check if our current solution is one solution of the Problem
	 * @param p the problem we are currently solving.
	 * @return true if the problem is solved, false otherwise.
	 */
	bool isSolution(Problem p) const;
	

	/**
	 * This method is use to solve a Problem
	 * @param p the problem we try to solve.
	 */
	void solve(Problem p);
	
	/**
	 * This method is use to inverse the i-th value in our solution.
	 * @param i the index of the solution we want to inverse.
	 */
	inline void inverse(unsigned int i);

	/** 
	 * To display a Strategie 
	 * @param out the out-stream where the text will be written (stdout by default)
	 */
	void display(ostream &out) const;


private:

	/** Our array of solutions. */
	int* arrayOfSolutions;

	/** The number of variable in the Problem. */
	size_t size;


};

	/** We redefined the output to display a clause during debugging phases. */
	std::ostream& operator<<(std::ostream& out, Solver const& Solver);



#endif
