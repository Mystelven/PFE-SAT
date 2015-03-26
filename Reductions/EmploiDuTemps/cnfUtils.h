/*
##############################################################################
# 
# TimeTable - Valentin Montmirail - Polytech Tours, France
# Copyright (c) 2015.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
##############################################################################*/

#ifndef CNF_UTILS_H
#define CNF_UTILS_H

#include "planning.h"

/**
 * readInputFile : This function will read the formalized input file
 * and will create a Planning structure with everything inside.
 * and ready to be transform into a CNF file.
 * @param filename the name of the input file.
 * @return a Planning structure. 
 */
Planning * readInputFile(const char* filename);

char* createCNF(Planning* planning);

unsigned int* getSolutionSchedule(Planning* planning,const char* s);

void displaySolutionSchedule(FILE* output, Planning* planning,unsigned int* solution);

inline unsigned int getNbVariables(Planning* planning);

int isSolutionExisting(Planning* planning, unsigned int* solution);

#endif 
