#!/bin/bash

rm -rf results/*

for fichier in `ls ./instances/uf20-91/` 
do
	echo "On va traiter le fichier: "$fichier;
	./satyr "./instances/uf20-91/"$fichier > "results/"$fichier & 
	pid=$!;
	echo $pid;
	(sleep 2; kill $pid) &
	#(sleep 2; kill $pid) & 
	#sleep_pid = $

done

# killall satyr 2> /dev/null; time (./test.sh > /dev/null 2> /dev/null);  cat results/* | grep "SATI" | wc -l; killall satyr;