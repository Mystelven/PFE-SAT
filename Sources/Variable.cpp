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

#include "../Headers/Variable.h"
using namespace std;

/** 
 * It allows the constructor in 2 ways and then,
 * There is no need to worry about the order of the parameters to the constructor.
 *
 * This one is the num in first and the value in second.
 */
Variable::Variable(unsigned int i,bool v) {

	/** 
	* There is a complicated case when positive integers are defined.
	* Is that 0 is a positive integer, but a variable can not be the number 0.
	*/
	if(i == 0) {
		cerr << "There's a problem, we can have a 0th Variable\n";
		exit(-1);
	}

	num = i;
	value = v;
}

Variable::Variable(unsigned int i) {

	value = true;
	num = i;
}

/** 
 * It allows the constructor in 2 ways and then,
 * There is no need to worry about the order of the parameters to the constructor.
 *
 * This one is the value in first and the num in second.
 */
Variable::Variable(bool v,unsigned int i) {

	/** 
	* There is a complicated case when positive integers are defined.
	* Is that 0 is a positive integer, but a variable can not be the number 0.
	*/
	if(i == 0) {
		cerr << "There's a problem, we can have a 0th Variable\n";
		exit(-1);
	}

	num = i;
	value = v;
}

/** 
 * It gives the possibility to reverse a variable.
 * a true variable become a false variable.
*/
void Variable::reverse() {

	value = !value;
}



/**
* to get the num of the variable.
*/
unsigned int Variable::getNum() const {

	return num;
}

/**
* to get the value of the variable.
*/
bool Variable::getValue() const {

	return value;
}

/**
* to display a variable.
*/
void Variable::display(ostream &out) const
{
	if(value == true)
		out << "";
	else
		out << "-";
	out << num;
}

/** 
* To test if 2 variables are the same 
*/
bool Variable::beEqual(Variable const& b) const
{
    return (num == b.num && value == b.value);
}

/** 
* To test if 2 variables are the same 
*/ 
bool operator==(Variable const& a, Variable const& b)
{
    return a.beEqual(b);
}

/** 
* To test if 2 variables aren't the same 
*/ 
bool operator!=(Variable const& a, Variable const& b)
{
    return !(a == b);
}

/**
* to display a variable.
*/
ostream& operator<<(ostream& out, Variable const& variable)
{
    variable.display(out);
    return out;
}
