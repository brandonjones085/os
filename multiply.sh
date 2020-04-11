#!/bin/bash 

#!/bin/bash 


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
