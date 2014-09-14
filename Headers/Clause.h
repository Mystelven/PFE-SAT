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

#ifndef Clause_H
#define Clause_H

/** A Clause is composed of variables */
#include "Variable.h"

using namespace std;


/**
 * Xij are what we call Variable and one "line" is a Clause.
 * @author Valentin.M
 * @version 1.0
 * @see Variable
 */
class Clause {

	public:
		
		/** We can create a clause with a number of variables. */
		Clause(unsigned int nbVariable);
		/** We can create a clause with an array of variables. */
		Clause(Variable* p);

		/** We can create a clause without any variable. */
		Clause();

		/** We can get the ident-th Variable */
		Variable* getVariables(unsigned int ident);

		/** We can add a Variable directly to our Clause. */
		void addVariable(Variable v);

		/** We can get the size of the array in the clause. */
		size_t size() const;

		/** to display a clause */
		void display(ostream &out) const;


	private:

		/** a clause is just an array of variables */
		Variable* arrayOfVariables;

		/** And we are able to get the number of Variable directly. */
		unsigned int nbVariables;
};
	
	/** We redefined the output to display a clause during debugging phases. */
	std::ostream& operator<<(std::ostream& out, Clause const& Clause);


#endif
