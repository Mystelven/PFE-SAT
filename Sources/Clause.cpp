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

#include "../Headers/Clause.h"
#include "../Headers/Solver.h"
using namespace std;

FileLogger log_clause (VERSION, "logs/clause.log");

/**
* It's the constructor of a clause.
* We will create N Variables which all 
*/
Clause::Clause(unsigned int n) {

	/** 
	* There is a complicated case when positive integers are defined.
	* Is that 0 is a positive integer, but a variable can not be the number 0.
	*/
	if(n == 0) {
		cerr << "There's a problem, we can have a Clause with 0 Variable.\n";
		exit(-1);
	}

	arrayOfVariables = static_cast<Variable*>(malloc(sizeof(Variable)*n));
 	if (arrayOfVariables == NULL)
 	{
   		log_clause << FileLogger::e_logType(FileLogger::LOG_ERROR)<< "The allocation of the arrayOfVariables has failed";
   		delete arrayOfVariables;
   		exit(-1);
 	}


	for(unsigned int i = 0; i < n; i++)
		arrayOfVariables[i] = Variable(i+1,false);

	nbVariables = n;
}

/**
 * This is the copy-constructor of a Clause.
 * It's needed because there's a pointer in the attribute of the class.
 * @param c the Original Clause that we will copy.
 */
Clause::Clause(Clause& c) {

	nbVariables = c.nbVariables;

	arrayOfVariables = static_cast<Variable*>(malloc(sizeof(Variable)*nbVariables));
 	if (arrayOfVariables == NULL)
 	{
   		log_clause << FileLogger::e_logType(FileLogger::LOG_ERROR)<< "The allocation of the arrayOfVariables has failed";
   		delete arrayOfVariables;
   		exit(-1);
 	}

	for(unsigned int i = 0; i < nbVariables; i++) {

		arrayOfVariables[i] = c.arrayOfVariables[i];
	}
}

/**
* It's the constructor of a clause.
* We will use directly an array of Variables.
*/
Clause::Clause(Variable* p) {

	unsigned int i = 0;
	if(p != NULL) {
		while((p+i) != NULL) {
			i++;
		}	
	}
	nbVariables = i;
	arrayOfVariables = p;
}

Clause::Clause() {

	nbVariables = 0;
	arrayOfVariables = NULL;
}

/** 
 * To get the variable number 'ident' in the clause.
 * If this variable doesn't exist, we return NULL.
 */
Variable* Clause::getVariables(unsigned int ident) {

	for(unsigned int i = 0 ; i < nbVariables; i++)
		if(arrayOfVariables[i].getNum() == ident) 
			return &arrayOfVariables[i];


	return NULL;

}

void Clause::addVariable(Variable v) {

	nbVariables++;
	Variable* new_data;

	if(arrayOfVariables == NULL) {
		new_data = static_cast<Variable*>(malloc(sizeof(Variable)*nbVariables));
		if (new_data == NULL)
 		{
   			log_clause << FileLogger::e_logType(FileLogger::LOG_ERROR) << "The allocation of the arrayOfVariables has failed";
   			delete arrayOfVariables;
   			exit(-1);
 		}
 		else
 		{
     		arrayOfVariables = new_data;
 		}
	}
	else {

		new_data = static_cast<Variable*>((realloc (arrayOfVariables, (nbVariables+10)*sizeof(Variable))));
 		if (new_data == NULL)
 		{
   			log_clause << FileLogger::e_logType(FileLogger::LOG_ERROR)<< "The allocation of the arrayOfVariables has failed";
   			delete arrayOfVariables;
   			exit(-1);
 		}
 		else
 		{
     		arrayOfVariables = new_data;
 		}

	}

	arrayOfVariables[nbVariables-1] = v;
}

/** To get the size of a clause. */
size_t Clause::size() const {

	log_clause << FileLogger::e_logType(FileLogger::LOG_INFO) << "size() -- IN";
	log_clause << FileLogger::e_logType(FileLogger::LOG_INFO) << "size() -- OUT";
	return nbVariables;
}


/**
* To display a clause.
*/
void Clause::display(ostream &out) const {

	for(unsigned int i = 0; i < nbVariables; i++)
		out << arrayOfVariables[i] << " ";

}

/**
* To display a clause.
*/
ostream& operator<<(ostream& out, Clause const& clause)
{
    clause.display(out);
    return out;
}
