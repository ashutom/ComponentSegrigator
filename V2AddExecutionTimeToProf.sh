#!/bin/bash


#constants for using
CWF="$PWD/workingdir"
ETF="ExecutionTime"
NFET="$1_with_$ETF.csv"
EXTM="NONE"  #Execution Time Method
CPPEXE="FileProcessor"
CPPBUILD_DIR="build"
OUT_FILE=""

echo
echo
echo "Profiler Output File given = $1 "

if [ ! -e "$1" ] || [ ! -s "$1" ] ; then
    echo "File is empty or does not exists "
    exit 1
fi

if [[ -z "$2" ]] ; then
    echo " Output File Name is not provided hence keep it to be Processed_File.csv "
    OUT_FILE="Processed_File.csv"
else
    OUT_FILE="$2"
fi


echo
echo
echo "Output File to be created ==  $OUT_FILE "

echo
echo "Creating working dir "
mkdir -p $CWF

if [ $? -ne 0 ] ;then
    echo "Could not create Directory "
    exit 1
fi

#build CPP code
if [ ! -e "$CPPEXE" ] || [ ! -s "$CPPEXE" ] ; then
	mkdir -p $CPPBUILD_DIR
	cd $CPPBUILD_DIR
	cmake ..
	make
	cp $CPPEXE ../
	cd ..
	rm -rf $CPPBUILD_DIR
fi


if [ ! -e "$CPPEXE" ] || [ ! -s "$CPPEXE" ] ; then
    echo "Fatal !!!   $CPPEXE is empty or does not exists "
    exit 1
fi



#debug info of cpp processor
#export AMF_DEBUG=1

./$CPPEXE 5 "$1" "$OUT_FILE"


if [ $? -eq 0 ]; then
    echo "Sucessfully Completed"
else
    echo "There was an error in processing "
    exit 1
fi

echo
echo "Removing temps "
rm -rf $CWF


unset AMF_DEBUG

echo
echo

if [ $? -eq 0 ]; then
    echo "Sucessfully Completed"
    exit 0
else
    echo "There was an error in processing "
    exit 1
fi







#BKUP STUFF

#cat prof.csv  | rev  > reversed_prof.csv
#cut -d',' -f1 reversed_prof.csv  > last_col_rev
#cut -d',' -f4 reversed_prof.csv  > last_to_4_col_rev
#cat last_col_rev | rev > last_col
#cat last_to_4_col_rev | rev > last_to_4_col
#paste last_to_4_col last_col  | awk '{print $2-$1}'  > execution_time

