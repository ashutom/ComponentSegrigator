#!/bin/bash


#constants for using
CWF="$PWD/workingdir"
ETF="ExecutionTime"
NFET="$1_with_$ETF.csv"
EXTM="NONE"  #Execution Time Method
CPPEXE="FileProcessor"
CPPBUILD_DIR="build"


echo
echo
echo "Profiler Output File given = $1 "

if [ ! -e "$1" ] || [ ! -s "$1" ] ; then
    echo "File is empty or does not exists "
    exit 1
fi


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


declare -a ExecutionOptions=("Execution Time = Endtime - StartTime" "Execution Time = CompletionTime -  SubmissionTime")
declare -a ExecutionTimeEXTM=("End_Start" "Complete_Dispatch")

echo
echo "Calculating Execution Time "


LoopCounter=0
OutputFiles=()

for i in "${ExecutionOptions[@]}"
do
    echo
    echo "$i"
    EXTM="${ExecutionTimeEXTM[$LoopCounter]}"
    echo
    echo "Processing ..... "
    echo

    if [ $LoopCounter -eq 0 ]; then
        awk -F, '{ print $(NF-1) - $(NF-2) }' $1 > $CWF/$ETF
    elif [ $LoopCounter -eq 1 ]; then
         awk -F, '{ print $NF - $(NF-3) }' $1 > $CWF/$ETF
    fi

    ((LoopCounter=LoopCounter+1))
    echo
    echo "Done .. "

    #change the header of the ExecutionTime
    sed -i "1s/.*/$ETF/" $CWF/$ETF

    NFET="$1_with_$ETF$EXTM.csv"

    echo
    echo "Adding Execution Time to Profiler Data ..."
    paste -d',' $CWF/$ETF $1 > $NFET
    echo
    echo "Done... "
    echo
    echo "Output File is $NFET "

    OutputFiles+=("$NFET")
done


#debug info of cpp processor
#export AMF_DEBUG=1

./$CPPEXE 4 "${OutputFiles[0]}" "${OutputFiles[1]}"

unset AMF_DEBUG

if [ $? -eq 0 ]; then
    echo "Sucessfully Completed"
else
    echo "There was an error in processing "
    exit 1
fi

echo
echo "Removing temps "
rm -rf $CWF
rm ${OutputFiles[0]}
rm ${OutputFiles[1]}


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

