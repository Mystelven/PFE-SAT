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

#include "../Headers/Problem.h"
using namespace std;
#include <ctime>
#include <unistd.h>


clock_t parse_time = 0;

/**
* It's the constructor of a problem.
* We will use directly an array of Clause. 
* @param p an array of Clauses.
* @param nClauses the number of clauses.
* @param nVariables the number of litterals.
*/
Problem::Problem(Clause* p,unsigned int nClauses,unsigned int nVariables) {

	arrayOfClauses = p;
	nbClauses = nClauses;
	nbVariables = nVariables;
}

/**
 * It's the constructor of a Problem.
 * We will use a CNF file to construct the Problem.
 * @param filename a CNF file to construct the Problem.
 */
Problem::Problem(string filename) {

	string buffer;  // we will read the file , line per line.
	string tmp;
	unsigned int var_tmp;
	unsigned int i = 0;

	ifstream file(filename.c_str(),ios::in);

	if (!file) {
		cerr << "There is a problem with the file : "<< filename << " you should verifiy the path.";
		cerr << endl << "Or maybe the right on the file, we should have the right to read the file." << endl << endl;
 		exit(-2);
	}
 	

 	cout << endl << endl;

  	clock_t start_time = clock();
    while(getline(file, buffer)) { // while we can get the line and we put it in buffer

 	   if(buffer[0] == 'c')		   // the 'c' in the first place, for CNF files, is the beginning of a comments.
 	   	    continue;			   // we don't need to parse comments.


 	   if(buffer[0] == 'p') {

 	   		istringstream iss(buffer);	// [ p cnf nbVariables nbClauses ]
 	   		iss >> tmp;					// [ p 							 ]
  			iss >> tmp;					// [   cnf  					 ]
  			iss >> nbVariables;			// [       nbVariables           ]
  			iss >> nbClauses;			// [  		  		   nbClauses ]

  			arrayOfClauses = (Clause*)malloc(sizeof(Clause)*nbClauses);

 	   } else {								// when it's not the definition of the Problem, it's the problem itself.

 	   		istringstream iss(buffer);
 	   		Clause c;
 	   		while(iss >> var_tmp) {			// we get the value of the variable

 	   			if(var_tmp != 0) {			// sometimes, there's some 0 in the file. But it's not a variable

 	   				Variable v(var_tmp);	// we create a object Variable. if var_tmp < 0 , v is false
 	   				c.addVariable(v);		// whereas if var_tmp > 0 , v is true. and v.num is the abs(var_tmp).

 	   			}
 
 	   		}

 	   		if(c.size() > 0) {				// if the line is just a 0, we don't add it to the Problem.
 				arrayOfClauses[i] = c;	    
 				i++; 						// we will add the next clause.
 			} 
 	   }


 	}
 	clock_t end_time = clock();
 	parse_time = (end_time - start_time);
 	nbClauses = i;							// just in case that the number of clause at the beginning of the file
	file.close();							// itsn't the same that the number of real clause added in the Problem.
}

/**
 * To get the number of Clauses in the Problem.
 * @return the number of Clauses.
 */
unsigned int Problem::getNbClauses() const {

	return nbClauses;
}

/**
 * To get the I-th Clauses of the Problem.
 * @param i the index of the Clause.
 * @return a pointer to the Clause.
 */
Clause* Problem::getClause(unsigned int i) {

	return &arrayOfClauses[i];
}


/**
 * To get the number of Variables in the Problem.
 * @return the number of variables.
 */
unsigned int Problem::getNbVariables() const {

	return nbVariables;
}


/**
* To display a Problem.
* @param out the out stream
*/
void Problem::display(ostream &out) const {

	unsigned int var = nbVariables;
	unsigned int clauses = nbClauses;

out << "c T=======================================[ Problem Statistics ]==========================================T" << endl;
out << "c |                                                                                                       |" << endl;
out << "c |  Number of variables : ";

if(var < 10)
	out << "         " << var;
else if(var < 100)
	out << "        " << var;
else if(var < 1000)
	out << "       " << var;
else if(var < 10000)
	out << "      " << var;
else
	out << "     " << var;

out << "                                  	                                  |" << endl;
out << "c |  Number of clauses   : ";

if(clauses < 10)
	out << "         " << clauses;
else if(clauses < 100)
	out << "        " << clauses;
else if(clauses < 1000)
	out << "       " << clauses;
else if(clauses < 10000)
	out << "      " << clauses;
else
	out << "     " << clauses;


out << "                                                                     |" << endl;
out << "c |  Parse time          : ";

out << "  ";
if(parse_time/1000.0 < 1) out << " ";
out << fixed << setprecision(3) << (parse_time/1000.0);

out << "ms                                                                     |" << endl;

out << "c |                                                                                                       |" << endl;
out << "c |                                                                                                       |" << endl;
out << "c |                                                                                                       |" << endl;
out << "c |                                                                                                       |" << endl;
out << "c |=======================================================================================================|" << endl;

}

/**
* To display a Problem.
* @param out the out-stream
* @param problem the displayed problem.
* @return the out-stream (to be recursive) like cout << problem << problem ... 
*/
ostream& operator<<(ostream& out, Problem const& problem)
{
    problem.display(out);
    return out;
}



