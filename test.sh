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

	testUf $1 "uf20/" 20 1000
	testUf $1 "uf50/" 50 1000
	testUf $1 "uf75/" 75 1000
	testUf $1 "uf100/" 100 1000
	#testUf $1 "uf125/" 125 1000
	#testUf $1 "uf150/" 150 1000
	#testUf $1 "uf175/" 175 1000
	#testUf $1 "uf200/" 200 1000
	#testUf $1 "uf225/" 225 1000
	#testUf $1 "uf250/" 250 1000
	#testUf $1 "uuf50/" 50 1000
	#testUf $1 "uuf100/" 100 1000
	#testUf $1 "uuf200/" 200 1000

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

directory=$2;
directory=`echo $directory | rev | cut -c 2- | rev`;

rm "sortie.txt";

nbInstances=$4;

for fichier in `ls $path | cut -f 2 -d '-' | sort -n | head -$nbInstances` 
do
	
	file="$path$directory";
	#file=$file"100";
	file=$file"-$fichier";
	
	echo $solver." : ".$file;

	("./"$solver $file >> "sortie.txt")

done

end=`date +%s%N`

total=$(ls $path | wc -l | tr -cd '[[:digit:]]')
total=$(printf "%04d" $total)

resultat=$(cat sortie.txt | grep "SATISFIABLE" | wc -l | tr -cd '[[:digit:]]')
resultat=$(printf "%04d" $resultat)

heure=$((end-start))
heure=$((heure/1000000));
heureF=$heure;
heure=$(printf "% 5d" $heure)

echo "        $solver : $resultat instances en $heure ms";

echo $(cat sortie.txt | grep "SATISFIABLE" | wc -l | tr -cd '[[:digit:]]') >> "$1_results.txt"
echo $heureF >> "$1_temps.txt"
}

if [ $# -eq 1 ]
then
	test $1
else

##############################################################################

test "zchaff"

##############################################################################

#test "walksat"

##############################################################################

test "glucose"

##############################################################################

test "satyr"

##############################################################################

#test "gaSAT"

##############################################################################

fi

##############################################################################

rm error.log
echo "================================================"

###############################################################################
