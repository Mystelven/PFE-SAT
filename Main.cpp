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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Headers/Solver.h"

using namespace std;

/** We declare a global logger for the file. */
FileLogger log_main (VERSION, "logs/main.log");

/**
 * Useful to display some informations about the inspiration (Glucose) and about the solver itself.
 * @param out the display-stream
 * @param p the current Problem.
 */
void displayInfo(ostream& out,Problem p) {

  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displayInfo -- IN";

	out << "c This is SATyr " << VERSION << endl;
	out << "c Created by Valentin Montmirail, a student in computer engineering at Polytech Tours" << endl;
	out << "c" << endl;
	out << p;

  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displayInfo -- OUT";
}

/**
 * Useful to display some error during the parsing of the CNF file.
 * @param out the display-stream
 */
void displayErrorArgument(ostream& out) {

    log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displayErrorArgument -- IN";

  	out << endl << endl;
  	out << "You should give the path to the CNF file formated as follow :  " << endl;
  	out << "p cnf nbVariables nbClauses" << endl;
  	out << "xi xj xk ..." << endl;
  	out << "xi xl ... " << endl;
  	out << endl << "so your call looks like : ./Solver path_to_file" << endl << endl ;

    log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displayErrorArgument -- OUT";
}

/**
 * Useful to display s SATISFIABLE and the solution or s INSATISFIABLE otherwise.
 * @param out the display-stream
 * @param s the value of each Variable in the Solver, the current Problem
 * @param p the current Problem.
 */
void displaySolution(ostream& out,Solver s,Problem p) {

    log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displaySolution -- IN";

    if(s.isSolution(p)) {
      
      log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "The problem is SATISFIABLE. ";

      out << endl << "s SATISFIABLE" << endl;
      out << "v " << s << " 0" << endl << endl;

    } else {
      log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "The problem is INSATISFIABLE. ";
      out << endl << "s INSATISFIABLE" << endl;

    }

    log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displaySolution -- OUT";
}

/**
 * Useful to display the time to solve this instance.
 * @param out the display-stream
 * @param solveTime the time to solve the current instance.
 */
void displaySolveTime(ostream& out,double solveTime) {

  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displaySolveTime -- IN";

  out << "c [=======================================================================================================]" << endl;

  solveTime *= 10000000;
  solveTime = ceil(solveTime);
  solveTime /= 10000000;

  std::ostringstream oss;
  oss << "Time to solve the problem: " << fixed << setprecision(6) << solveTime << "s";
  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << oss.str();

  out << endl;
  out << "c CPU Time              :    " << fixed << setprecision(6) << solveTime << "s";
  out << endl;

  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "displaySolveTime -- OUT";

}

/**
 * It's the beginning of this program.
 * @param argc the number of argument for the main fonction
 * @param argv each argument in a char*
 * @return a value between [-127 ; 128] to explain the exit of the program.
 */
int main(int argc,char** argv)
{
  
  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "main -- IN";


  clock_t start,end;
  srand(time(NULL)); // initialisation of rand

  if(argc < 2) {

  	displayErrorArgument(cerr);
  	exit(-3);

  }

  std::ostringstream oss;
  oss << "Fichier: " << argv[1];
  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << oss.str();
  
  Problem p(argv[1]);
  Solver s(p);

  displayInfo(cout,p);

  start = clock();
  s.solve(p);
  end = clock();    // if we don't catch an Exception, 
                    // then the problem is SATISFIABLE or INSATISFIABLE

  double solveTime = (end-start)/1000000.0;
  displaySolveTime(cout,solveTime);
  displaySolution(cout,s,p);

  if(argc > 2) {
    
    ofstream outfile(argv[2],ofstream::binary);

    if(!outfile) {

      // if we can't create the file, we will use the standard output.
      cerr << "There is a problem, we can create the file." << endl;
      cerr << "Try to fix it by yourself, and copy the solution into your new file (or re-execute SATyr)" << endl;

    } else {

      displaySolution(outfile,s,p);       
      outfile.close();                      // At the end, we close the file (if we used it).

    }

  }

  log_main << FileLogger::e_logType(FileLogger::LOG_INFO) << "main -- OUT";

  return 0;
}
