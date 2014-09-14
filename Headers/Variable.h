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

#ifndef Variable_H
#define Variable_H

#include <iostream>
using namespace std;


/**
 * each x is a variable or a negation of a variable, 
 * and each variable can appear multiple times in the Problem.
 * @author Valentin.M
 * @version 1.0
 */
class Variable {

	public:
		
		/** 
		* It allows the constructor in 2 ways and then,
		* There is no need to worry about the order of the parameters to the constructor.
		*/
		Variable(unsigned int i,bool v);
		Variable(bool v,unsigned int i);

		/** Constructor which can create a variable from a int and value always true */
		Variable(unsigned int i);	

		/** 
		* We create Getter and Setter in order to access and to modify the fields 
		* that are private and therefore inaccessible from the outside.
		*/
		inline void 		  setValue(bool v);
		inline void 		  setNum(unsigned int i);
		
		unsigned int  getNum() const;
		bool 		  getValue() const;

		/** 
		* It gives the possibility to reverse a variable.
		* a true variable become a false variable.
		*/
		inline void reverse();

		/** 
		We create methods of the class to redefine operators to gain access to private fields.
		*/	
		void display(ostream &out) const;
		inline bool beEqual(Variable const& b) const;


	private:

		/** a variable is either true or false. */
		bool value;				

		/** a variable has a number to identify itself */
		unsigned int num;		

};
	
	/** We redefined the output to display a variable during debugging phases. */
	std::ostream& operator<<(std::ostream& out, Variable const& Variable);

	/** We redefined the equalities's test to test if the variables are the same or not. */
	bool operator==(Variable const& a, Variable const& b);
	bool operator!=(Variable const& a, Variable const& b);

#endif
