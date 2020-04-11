#VARIABLES
row=0
col=0
i=0
tot=0
count=1

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
    echo -n  $myLine2 | tr -d " " >> $temp
done <$2


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



#loops through the values, cuts the first value and total + 1 for the sum
#this continues for all the values 
#places sums in sumTotal
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    for i in $(seq 1 $tot)
    do
    sum=$(expr $( cut -c $i $temp ) + $(cut -c $(expr $tot + $i ) $temp )) 
    IFS=$'\r\n' GLOBIGNORE='*' command eval  'sumArray+=($sum)'
    done

    echo $myLine
    
done <$temp


#This is used to format array back into matrix
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
rm -f "temp"