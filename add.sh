#!/bin/bash 


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
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    echo  -n $myLine |  tr -d " " >> $temp

done <$2

#this block checks both matricies are the same size
if [ $row != $row2 ]
then 
    echo "ERROR: Matricies need to have the same number col and rows" 1>&2
    exit 1
fi  

if [ $col != $col2 ]
then 
    echo "ERROR: Matricies need to have the same number col and rows" 1>&2
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









