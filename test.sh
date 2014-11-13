#!/bin/bash

rm -rf results/*

for fichier in `ls ./instances/uf20-91/` 
do
	echo $fichier;
	./satyr "./instances/uf20-91/"$fichier > "results/"$fichier&
	pid=$!;
	(sleep 2; kill $pid) &

done

# killall satyr 2> /dev/null; time (./test.sh > /dev/null 2> /dev/null);  cat results/* | grep "SATI" | wc -l; killall satyr;

# ./test.sh 2> /dev/null ; cat results/* | grep "SATI" | wc -l ;