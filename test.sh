#!/bin/bash

###############################################################################
rm -rf results/*

solver="satyr"

start=`date +%s%N`
#for fichier in `ls ./instances/uf20/` 
#do
	#fichier="uf20-01.cnf";
	#file="./instances/uf20/$fichier";
	#(exec "./$solver" "$file" > "./results/$fichier")& 
#done
end=`date +%s%N`

total=$(ls ./instances/uf20/ | wc -l | tr -cd '[[:digit:]]')
#resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
resultat="0000";

heure=$((end-start))
heure=$((heure/1000000));

echo ""
echo "    $solver   : $resultat/$total instances en $heure ms";

##############################################################################

rm -rf results/*

solver="glucose"

start=`date +%s%N`
for fichier in `ls ./instances/uf20/` 
do
	file="./instances/uf20/$fichier";
	(exec "./$solver" "$file" > "./results/$fichier")& 
done
end=`date +%s%N`

total=$(ls ./instances/uf20/ | wc -l | tr -cd '[[:digit:]]')
resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')

heure=$((end-start))
heure=$((heure/1000000));

echo ""
echo "    $solver : $resultat/$total instances en $heure ms";
echo ""

##############################################################################

# killall satyr 2> /dev/null; time (./test.sh > /dev/null 2> /dev/null);  cat results/* | grep "SATI" | wc -l; killall satyr;

# ./test.sh 2> /dev/null ; cat results/* | grep "SATI" | wc -l ;