/***************************************************************************************n***********
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

#include "../Headers/Solver.h"
using namespace std;
#include <ctime>

/**
 * We create a Solver for a precise Problem.
 */
Solver::Solver(Problem p) {

	arrayOfSolutions = (int*)malloc(sizeof(int)*p.getNbVariables());
	
	size = p.getNbVariables();

	for(unsigned int i= 0; i < size ; i++)
		arrayOfSolutions[i] = 0;

}

/**
* To display a Solver.
*/
void Solver::display(ostream &out) const {

   for(unsigned int i = 0; i < size ; i++) {
   		if(arrayOfSolutions[i] < 0) out << "-" << (i+1) << " ";
   		else if(arrayOfSolutions[i] > 0) out << (i+1) << " ";
   		else out << " ? ";
   	}

}

/**
 * to get the number of Variable ( in the Solver class )
 */
size_t Solver::getSize() const {

	return size;
}

/** 
 * to get the negation of a variable 'i' 
 */
void Solver::inverse(unsigned int i) {

	if(arrayOfSolutions[i-1] == 0) 
		arrayOfSolutions[i-1] = 1;

	arrayOfSolutions[i-1] *= -1;
}


/** 
   to solve a Problem.
   will use a design pattern Strategy to be more efficient
   on each type of Problem.
*/
void Solver::solve(Problem p) {

  unsigned int var;
  size_t v_size = getSize();

	while(!isSolution(p)) {

     	   var = ((unsigned int)(rand())%v_size + 1);
    	   inverse(var);
	}	
}

/** 
 * to get the value of the variable 'i' in the Solver
 */
int Solver::getTheSolution(unsigned int i) const {

	if(i < size)
		return arrayOfSolutions[i];
	else {
		cerr << "There's a problem, you want the ith value of the Solver which doesn't exist";
		cerr << endl << endl;
		exit(-1);
	}
	return 0;
}

/**
 * to check if the Clause c is open with the Current Solver
 */
bool Solver::isOpen(Clause c) const {

	for(unsigned int i = 0; i < size; i++) {
		Variable* v = c.getVariables(i+1);
		if(v != NULL) {
			if(v->getValue() == true && arrayOfSolutions[i] == 1) 
					return true;	
			else if(v->getValue() == false && arrayOfSolutions[i] == -1)
					return true; 
		}
	}
	
	return false;
}

/** 
 * to check if the Problem is solved with the Current Solver
 */
bool Solver::isSolution(Problem problem) {

	for(unsigned int i= 0; i < problem.getNbClauses(); i++)
		if(!isOpen(*(problem.getClause(i)))) 
			return false;

	return true;
}


/**
* To display a Solver.
*/
ostream& operator<<(ostream& out, Solver const& Solver)
{
    Solver.display(out);
    return out;
}

