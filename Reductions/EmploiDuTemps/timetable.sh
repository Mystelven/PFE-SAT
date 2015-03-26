#!/bin/bash

clear;
echo "[INFO] - Welcome in Class Scheduling to SAT by Valentin Montmirail";
echo "[INFO] - Let's compilate everything to be sure that everything is up-to-date"
echo " ";

make clean; 
make;
echo " ";

echo "[INFO] - We generate our input file";
echo "[INFO] - ./generator lol2.input";

./generator lol2.input

echo "[INFO] - Then we solve it and create our HTML result"
echo "[INFO] - time ./timetable lol2.input ../../satyrSAT";

time ./timetable lol2.input ../../satyrSAT

echo " ";
echo "[INFO] - We finally open the result in a browser"
echo "[INFO] - open output.html";

open output.html

echo " "
