#!/bin/bash

echo "Profiler Output File given = $1 "

if [ ! -e "$1" ] || [ ! -s "$1" ] ; then
    echo "File is empty or does not exists "
    exit 1
fi

#constants for using
CWF="$PWD/workingdir"
ETF="ExecutionTime"
NFET="$1_with_$ETF.csv"
EXTM="NONE"  #Execution Time Method


echo
echo "Creating working dir "
mkdir -p $CWF

if [ $? -ne 0 ] ;then
    echo "Could not create Directory "
    exit 1
fi

echo
echo "Calculating Execution Time "
echo
echo "Options for calculating the execution time "
echo
echo "1. Execution Time = Endtime - StartTime "
echo "2. Execution Time = CompletionTime -  SubmissionTime "
echo
echo "Choice ( 1/2 ) "

read CHOICE

echo
echo "Processing ..... "
echo

if [ $CHOICE -eq 1 ]; then
     EXTM="End_Start"
     awk -F, '{ print $(NF-1) - $(NF-2) }' $1 > $CWF/$ETF
elif [ $CHOICE -eq 2 ]; then
     EXTM="Complete_Dispatch"
     awk -F, '{ print $NF - $(NF-3) }' $1 > $CWF/$ETF
else
   echo "Wrong choice "
   exit 1
fi

NFET="$1_with_$ETF$EXTM.csv"

echo "Done .. "

sed -i "1s/.*/$ETF/" $CWF/$ETF


echo
echo "Adding Execution Time to Profiler Data ..."
paste -d',' $CWF/$ETF $1 > "$NFET"
echo "Done... "


echo
echo "Removing temps "
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

