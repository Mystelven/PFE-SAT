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

#ifndef PROBLEM_H
#define PROBLEM_H

/** A Problem is composed of clauses */
#include "Clause.h"

#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

/**
 * the complete boolean expression is the Problem.
 * @author Valentin.M
 * @version 1.0
 * @see Variable
 * @see Clause
 */
class Problem {

public:

		/** We can create a Problem with an array of Clause. */
		Problem(Clause* p,unsigned int n,unsigned int n2);

		/** We can create a Problem directly from a CNF file. */
		Problem(string filename);

		/** We can get the number of Variables in the Problem.*/
		unsigned int getNbVariables();

		/** We can get the number of Clauses in the Problem. */
		unsigned int getNbClauses();

		/** We can get a reference to the i-th Clause of the Problem. */
		Clause* getClause(unsigned int i);

		/** To display a Problem */
		void display(ostream &out) const;


private:

	/** A Problem is just an array of Clauses. */
	Clause* arrayOfClauses;

	/** 
	 * We need to have a look on the number of clauses and the number of variables.
	 * Because it's the problem itself.
	 */
	unsigned int nbClauses;
	unsigned int nbVariables;

};


	/** We redefined the output to display a clause during debugging phases. */
	std::ostream& operator<<(std::ostream& out, Problem const& problem);


#endif
