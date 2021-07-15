#!/bin/bash

echo " Profiler Output File given = $1 "

if [ ! -e "$1" ] || [ ! -s "$1" ] ; then
    echo "File is empty or does not exists "
fi

#constants for using
CWF="$PWD/workingdir"
ETF="ExecutionTime"
NFET="$1_with_$ETF.csv"


echo
echo "Creating working dir "
mkdir -p $CWF

if [ $? -ne 0 ] ;then
    echo "Could not create Directory "
    exit 1
fi

echo
echo "Calculating Execution Time "
awk -F, '{ print $NF - $(NF-3) }' $1 > $CWF/$ETF
echo " Done .. "

sed -i "1s/.*/$ETF/" $CWF/$ETF


echo
echo "Adding Execution Time to Profiler "
paste -d',' $CWF/$ETF prof.csv > "$NFET"
echo " Done... "


echo
echo " Removing temps "
rm -rf $CWF

echo "Sucessfully Completed"

echo "Output File is $NFET "












#BKUP STUFF

#cat prof.csv  | rev  > reversed_prof.csv
#cut -d',' -f1 reversed_prof.csv  > last_col_rev
#cut -d',' -f4 reversed_prof.csv  > last_to_4_col_rev
#cat last_col_rev | rev > last_col
#cat last_to_4_col_rev | rev > last_to_4_col
#paste last_to_4_col last_col  | awk '{print $2-$1}'  > execution_time

