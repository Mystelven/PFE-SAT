#!/bin/bash

###############################################################################
rm -rf results/*
touch error.log

solver="satyr"

start=`date +%s%N`
for fichier in `ls ./instances/uf20/` 
do
	file="./instances/uf20/$fichier";	
	(exec "./$solver" "$file" > "./results/$fichier")
done
end=`date +%s%N`
echo "================================================"

(exec "killall $solver") 2> "error.log" > "error.log"

total=$(ls ./instances/uf20/ | wc -l | tr -cd '[[:digit:]]')
resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
resultat=$(printf "%04d" $resultat)

heure=$((end-start))
heure=$((heure/1000000));
heure=$(printf "% 5d" $heure)

echo "    satyr   : $resultat/$total instances en $heure ms";

##############################################################################
rm -rf results/*
solver="walkSAT"

start=`date +%s%N`
for fichier in `ls ./instances/uf20/` 
do
	file="./instances/uf20/$fichier";
	(exec "./$solver" "$file" > "./results/$fichier")& 
done
end=`date +%s%N`

(exec "killall $solver") 2> "error.log" > "error.log"

total=$(ls ./instances/uf20/ | wc -l | tr -cd '[[:digit:]]')
resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
resultat=$(printf "%04d" $resultat)

heure=$((end-start))
heure=$((heure/1000000));
heure=$(printf "% 5d" $heure)

echo "    $solver : $resultat/$total instances en $heure ms";

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

(exec "killall $solver") 2> "error.log" > "error.log"

total=$(ls ./instances/uf20/ | wc -l | tr -cd '[[:digit:]]')
resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')

heure=$((end-start))
heure=$((heure/1000000));
heure=$(printf "% 5d" $heure)

echo "    $solver : $resultat/$total instances en $heure ms";
echo "================================================"

rm error.log
##############################################################################

# killall satyr 2> /dev/null; time (./test.sh > /dev/null 2> /dev/null);  cat results/* | grep "SATI" | wc -l; killall satyr;

# ./test.sh 2> /dev/null ; cat results/* | grep "SATI" | wc -l ;