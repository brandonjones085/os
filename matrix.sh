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

if [[ -r $1 ]]
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
    exit 1 
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
row2=0
col2=0
tot=0
i=0
j=0
count=1


#FILES
temp1="temp1"
temp2="temp2"
sumTotals="sumTotals"
one="one"
two="two"

if [ $# -eq 0 ]
then 
    echo "ERROR: Need at least one matrix" 1>&2
    exit 1

fi

#reads in first matrix and gets the number of rows and col
#puts the matrix into a single line, removing all spaces into the temp
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows
    col=$(expr $col + 1)           #counts the number of cols
    echo -e $myLine >> $temp1
   
done <$1

#replaces spaces with new line
cat $temp1 | tr " " "\n" >> one 


#finds the total number of values 
tot=$(($row*$col))

#reads in the second matrix 
#places all values in a single line behind the first matrix and removes all spaces
while IFS= read -r myLine2 || [ -n "$myLine2" ]
do  
    row2=$( head -n 1 $2 | wc -w )   #number of rows
    col2=$(expr $col2 + 1)           #counts the number of cols
    echo -e $myLine2 >> $temp2

done <$2

#replaces spaces with new line
cat $temp2 | tr " " "\n" >> two 


#this block checks both matricies are the same size
if [[ $row != $row2 ]]
then 
    echo "ERROR: Matricies need to have the same number col and rows" 1>&2
    exit 1
fi  

if [[ $col != $col2 ]]
then 
    echo "ERROR: Matricies need to have the same number col and rows" 1>&2
    exit 1
fi 


#loops through the values
#places sums in sumTotal
while read mat1 && read mat2 <&3
do 
   
    sum=$(expr $mat1 + $mat2 )
    #sum=$(expr $( cut -c $j $temp ) + $(cut -c $(expr $tot + $j ) $temp )) 
 
    IFS=$'\r\n' GLOBIGNORE='*' command eval  'sumArray+=($sum)'
    
done <$one 3<$two



# #This is used to format array back into matrix
while [[ $i -le $(expr $tot - 1) ]]   
do 
if [[ $count -ne $row ]]
then
    echo -n -e "${sumArray[$i]}\t"
    i=$(expr $i + 1 )
    count=$(expr $count + 1 )
    
else
    echo "${sumArray[$i]}"
    count=1
    i=$(expr $i + 1 )
fi 
done


rm -f "sumTotals"
rm -f "temp1"
rm -f "temp2"
rm -f "one"
rm -f "two"
}


multiply()
{

#VARIABLES
row1=0
col1=0
row2=0
col2=0
i=0
j=0

#FILES
temp="temp"
sumTotals="sumTotals"
temp2="temp2"
tempfile="tempfile"
tempfile2="tempfile2"


#reads in first matrix and gets the number of rows and col
#puts the matrix into a single line, removing all spaces into the temp
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row1=$( head -n 1 $1 | wc -w )   #number of rows
    col1=$(expr $col1 + 1 )           #counts the number of cols
    echo $myLine >> $temp
    
done <$1

#reads in first matrix and gets the number of rows and col
#puts the matrix into a single line, removing all spaces into the temp2
while IFS=$'\t' read -r -a n || [ -n "$n" ]
do  
    row2=$( head -n 1 $2 | wc -w )   #number of rows
    col2=$(expr $col2 + 1 )           #counts the number of cols
    echo -e $n >> $temp2
done <$2

#this runs if multiplication is possible 
if [ $row1 -eq $col2 ]
then


for i in $(seq 1 $(expr $col1 ) )
#for ((i=1; i<=col1; i++))
    do 
    cat $temp | tr " " "\t" | head -$i | tail -1 > tempfile

    for j  in $(seq 1 $(expr $row2 ) )

    do 
        count=1 
        sum=0
        cut -f $j $2  > tempfile2

        while IFS= read -r c || [ -n "$c" ]
        do 
            r=$( cut -f $count tempfile )
            mul=$((r*c))
            sum=$(expr $sum + $mul )
            count=$(expr $count + 1 )

        done < $tempfile2


        if [ $j -lt $row2 ]
        then 
            echo -e -n "$sum\t"
        else
            echo $sum

        fi 

    done 
done 
        
else
    echo "Error: The two matricies are not able to be multiplied"
    exit 1 
fi 



rm -f $temp
rm -f $temp2
rm -f $tempfile
rm -f $tempfile2


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