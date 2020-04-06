#!/bin/bash

row=0
col=0
tot=0
sum=0
num=0
mean=0
temp="temp"
testfile="testfile"
#This is mostly to just get the number of rows
#by counting the first row
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows
    col=$(expr $col + 1)           #counts the number of cols
    echo -n $myLine |  tr -d " " >> $temp

done <$1

#cats temp, 
cat $temp | fold -w1 > $testfile

while IFS= read -r num || [ -n "$num" ]
do  
    sum=$(expr $sum + $num )
done <$testfile

tot=$(($row*$col))
mean=$(($sum / $tot))
echo $mean
