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
echo 
echo $row", " $col
echo 
}

#TRANSPOSE FUNCTION
transpose()
{
row=0
i=1

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

}


#MEAN FUNCTION
mean()
{
#variable declaration
row=0
col=0
tot=0
sum=0
num=0
mean=0

#files
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


#cats temp, puts contents into testfile to prepare for sum
cat $temp | fold -w1 > $testfile


while IFS= read -r num || [ -n "$num" ]
do  
    sum=$(expr $sum + $num )
done <$testfile


tot=$(($row*$col))
mean=$(($sum / $tot))

echo $mean

}




#______________________Calling dims______________________
if [ $1 = 'dims' ]
then
    one=$2
    dims $one
fi

if [ $1 = 'transpose' ]
then
    one=$2
    transpose $one
fi 

if [ $1 = 'mean' ]
then
    one=$2
    mean $one
   
fi 


# if [ $1 = 'add' ]
# then
# fi 

# if [ $1 = 'multiply' ]
# then
# fi 