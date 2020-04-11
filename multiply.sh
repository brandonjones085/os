#!/bin/bash 

#!/bin/bash 


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
    echo "Error: The two matricies are able to be multiplied"
fi 


echo $row1 $col1
echo $row2 $col2