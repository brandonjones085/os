#!/bin/bash


#DIMS FUNCTION
dims()
{
col=0
row=0

while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )  #counts the number of rows
    col=$(expr $col + 1)           #counts the number of cols

done < $one

#returns values

echo $col $row


}

#TRANSPOSE FUNCTION
transpose()
{
row=0
i=1

if test -f $1 -a -f $1
then 


#This is mostly to just get the number of rows
#by counting the first row
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows

done < $one


for i in $(seq 1 $row)
do
#cats the file, cuts out each column, pastes the new row
cat  $1 | cut -f$i $1 | paste -s
done

else
    echo  "ERROR: File not readable" 1>&2
fi 

}


#MEAN FUNCTION
mean()
{

row=0
col=0
tot=0
sum=0
num=0
mean=0
i=1

temp="temp"
tempFile="tempFile"
final="final"
reversed="reversed"
revRemoved="revRemoved"

while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows
    col=$(expr $col + 1)           #counts the number of cols
    echo -n $myLine |  tr -d " " >> $temp
done <$1

for i in $(seq 1 $row)
do
cat  $temp | cut -f$i $1 | paste -s >> $tempFile

done

while IFS= read -r nums || [ -n "$nums" ]
do  
    sum=0
    for n in $nums
    do 
    sum=$(expr $sum + $n )

    #proper rounding formula
    mean=$(((sum + (col/2)*( (sum>0)*2-1 )) / col))

    done

    #replaces new line with tab moves to final 
    echo $mean | tr "\n" "\t" >> $final 
    
done <$tempFile


#This removes the ending tab by reversing the string
#then cutting out the first space
rev $final > $reversed
cat $reversed | cut -c 2- > $revRemoved
rev $revRemoved > $final 

cat $final 

rm -f $temp
rm -f $final
rm -f $revRemoved
rm -f $reversed 
rm -f $tempFile

}


add()
{

#VARIABLES
row=0
col=0
i=0
#FILES
temp="temp"
sumTotals="sumTotals"



#reads in first matrix and gets the number of rows and col
#puts the matrix into a single line, removing all spaces into the temp
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows
    col=$(expr $col + 1)           #counts the number of cols
    echo -n  $myLine | tr -d " " >> $temp
    
done <$1

#finds the total number of values 
tot=$(($row*$col))

#reads in the second matrix 
#places all values in a single line behind the first matrix and removes all spaces
while IFS= read -r myLine2 || [ -n "$myLine2" ]
do  
    row2=$( head -n 1 $2 | wc -w )   #number of rows
    col2=$(expr $col2 + 1)           #counts the number of cols

done <$2


#this block checks both matricies are the same size
if [[ $row != $row2 ]]
then 
    echo "1ERROR: Matricies need to have the same number col and rows" 1>&2
    exit 1
fi  

if [[ $col != $col2 ]]
then 
    echo "2ERROR: Matricies need to have the same number col and rows" 1>&2
    exit 1
fi 



#loops through the values, cuts the first value and total + 1 for the sum
#this continues for all the values 
#places sums in sumTotal
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    for i in $(seq 1 $tot)
    do
    sum=$(expr $( cut -c $i $temp ) + $(cut -c $(expr $tot + $i ) $temp )) 
    echo $sum >> $sumTotals
   
    done
    
done <$temp

IFS=$'\r\n' GLOBIGNORE='*' command eval  'sumArray=($(cat $sumTotals))'

for r in $(seq 1 $col )
do 
for c in $(seq 1 $row )
do
#echo -n ${sumArray[(($row*$r+$c-7))]}$'\t'
echo -n ${sumArray[(($row*$r+$c-$(expr $row + 1 )))]}$'\t'  
#echo -n ${sumArray[(($row*$r+$c-$(expr $col - $(expr $row - 1 ) )  ))]}$'\t'
done
echo 
done

#echo $(expr $col + $col - 1 )

#echo $(expr $row + 1 ) 


#echo ${sumArray[*]}

rm -f "sumTotals"
rm -f "temp"


}


multiply()
{
    #VARIABLES
row1=0
col1=0
row2=0
col2=0
i=0
#FILES
temp="temp"
sumTotals="sumTotals"
temp2="temp2"



#reads in first matrix and gets the number of rows and col
#puts the matrix into a single line, removing all spaces into the temp
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row1=$( head -n 1 $1 | wc -w )   #number of rows
    col1=$(expr $col1 + 1)           #counts the number of cols
    echo -n  $myLine | tr -d " " > $temp
    
done <$1


while IFS= read -r n || [ -n "$n" ]
do  
    row2=$( head -n 1 $2 | wc -w )   #number of rows
    col2=$(expr $col2 + 1)           #counts the number of cols
 
    
done <$2

    # for i in $(seq 1 $row2)
    # do
    # $temp2 < cat $2 | cut -f$i $2 | paste -s 
    # done

if [ $row1 != $col2 ]
then
    echo "Error: The two matricies are able to be multiplied" 1>&2
    exit 1
fi 


}



#______________________Calling dims______________________
if [ $1 = 'dims' ]
then
    if (( $# > 2 ))
    then 
     echo "ERROR: Only one matrix for DIMS function" 1>&2
     exit 1 
    fi 

    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least one matrix" 1>&2
    exit 1
    fi 

    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    if [ ! -f $2 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    if [ $# -gt 1 ]
    then 
    one=$2
    fi 

    dims $one

#____________________CALLING TRANSPOSE___________________

elif [ $1 = 'transpose' ]
then
     if (( $# > 2 ))
    then 
     echo "Error: Only one matrix for TRANSPOSE function" 1>&2
     exit 1 
    fi 

    # if [ ! -f $2 ]
    # then 
    #     echo "Error: No such file" 1>&2
    #     exit 1
    # fi 

    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    if [ $# -gt 1 ]
    then
    one=$2
    fi 

   
    transpose $one



#___________________CALLING MEAN______________________

elif [ $1 = 'mean' ]
then
        if (( $# > 2 ))
    then 
     echo "ERROR: Only one matrix for MEAN function" 1>&2
     exit 1 
    fi 

    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least one matrix" 1>&2
    exit 1
    fi 

    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    if [ ! -f $2 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    if [ $# -gt 1 ]
    then 
    one=$2
    fi 

    mean $one
   


elif [ $1 = 'add' ]
then
        if (( $# > 3 ))
    then 
     echo "ERROR: Only two matrices for ADD function" 1>&2
     exit 1 
    fi 

    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least two matricies" 1>&2
    exit 1
    fi 

    if [ ! -f $2 ] || [ ! -f $3 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    if [ $# -gt 2 ]
    then 
    one=$2
    two=$3
    fi 

    add $one $two 



elif [ $1 = 'multiply' ]
then

        if (( $# > 3 ))
    then 
     echo "ERROR: Only two matrices for MULTIPLY function" 1>&2
     exit 1 
    fi 

    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least two matricies" 1>&2
    exit 1
    fi 

    if [ ! -f $2 ] || [ ! -f $3 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 

     if [ $# -gt 2 ]
    then 
    one=$2
    two=$3
    fi 

    multiply $one $two 


else
    echo "ERROR: Badcommand" 1>&2
    exit 1

fi 