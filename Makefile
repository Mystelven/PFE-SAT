##############################################################################
# 
# Makefile for SATyr - Valentin Montmirail - Polytech Tours, France
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
##############################################################################

all: 
	(cd Glucose-dir/simp/ 	  ; make 					 ; 		 mv ./glucose 		 ../../ )
	(cd Glucose-dir/parallel/ ; make    				 ; 		 mv ./glucose-syrup  ../../ )
	(cd Satyr-dir/        	  ; make clean  ; make compet; 		 mv ./satyr      		../ )
	(cd Walksat-dir/     	  ; make 					 ; 		 mv ./walksat    		../ )
	(cd Zchaff-dir/       	  ; make 					 ; 		 mv ./zchaff     		../	)
	(cd GASAT-dir/        	  ; make 					 ; 		 mv ./gaSAT      		../ )
	(cd Cdls-dir/ 		  	  ; make compet 			 ; 		 mv ./cdls       		../ )
	(cd Reductions/ 	 	  ; gcc -o queens NQueens.c  ; 		 mv ./queens 			../ )
	(cd benchmarks/ 		  ; make 					 ; 		 mv ./benchmark 		../ )

clean:
	rm satyr
	rm walksat
	rm zchaff
	rm glucose
	rm gaSAT
	rm cdls
	rm queens
	rm glucose-syrup
	rm benchmark
