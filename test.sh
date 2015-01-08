#!/bin/bash

###############################################################################

touch error.log
echo "================================================"

echo ""
echo ""

rm    "Resultat.txt"

touch "Resultat.txt"

###############################################################################

function result()
{
	
	f=$(cat $1_temps.txt)
	temps=$(( ${f//$'\n'/+} ))

	f=$(cat $1_results.txt)
	nbInst=$(( ${f//$'\n'/+} ))

	echo "    $1 a résolu $nbInst instances en $temps ms"
	echo "$1 a résolu $nbInst instances en $temps ms" >> "Resultat.txt"
}

function test()
{	
	touch "$1_temps.txt"
	touch "$1_results.txt"

	testUf $1 "uf20/"
	#testUf $1 "uf50/"
	#testUf $1 "uf75/"
	#testUf $1 "uf100/"
	#testUf $1 "uf125/"
	#testUf $1 "uf150/"
	#testUf $1 "uf175/"
	#testUf $1 "uf200/"
	#testUf $1 "uf225/"
	#testUf $1 "uf250/"

	echo ""
	result $1 
	echo ""

	rm 	  "$1_temps.txt"
	rm    "$1_results.txt"
}

function testUf() 
{
rm -rf results/*
solver=$1;
path='./instances/'$2;
start=`date +%s%N`

sleepValue=0.3;
directory=$2;
directory=`echo $directory | rev | cut -c 2- | rev`;

for fichier in `ls $path | cut -f 2 -d '-' | sort -n` 
do
	
	file="$path$directory-$fichier";
	echo $file;
	(exec "./$solver" "$file" > "./results/$fichier")&
	sleep $sleepValue;
	#resultat=$(cat results/$fichier | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
	#if [ $resultat -eq 0 ]
	##then
	#	(exec "./$solver" "$file" > "./results/$fichier")
#		resultat=$(cat results/$fichier | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
#		if [ $resultat -eq 0 ]#
#		then
#			(exec "./$solver" "$file" > "./results/$fichier")&
#		fi
#	fi
done
end=`date +%s%N`

(exec "killall $solver") 2> "error.log" > "error.log"

total=$(ls $path | wc -l | tr -cd '[[:digit:]]')
total=$(printf "%04d" $total)

resultat=$(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]')
resultat=$(printf "%04d" $resultat)

heure=$((end-start))
heure=$((heure/1000000));
heureF=$heure;
heure=$(printf "% 5d" $heure)

echo "        $solver : $resultat/$total instances en $heure ms";

echo $(cat results/* | grep "SATI" | wc -l | tr -cd '[[:digit:]]') >> "$1_results.txt"
echo $heureF >> "$1_temps.txt"
}

if [ $# -eq 1 ]
then
	test $1
else

###############################################################################

test "walkSAT"

##############################################################################

test "zchaff"

##############################################################################

test "glucose"

##############################################################################

#test "satyr"

fi

##############################################################################

rm error.log
echo "================================================"

###############################################################################
