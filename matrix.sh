#!/bin/bash


#################################################################
#Name: DIMS 
#Purpose: Return dimensions of a matrix
#Input: A single matrix
#Output: The number of rows and columns
#Description: Takes in a matrix and returns the number of col and rows
##################################################################
#DIMS FUNCTION
dims()
{
col=0
row=0

#reads in file
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )  #counts the number of rows
    col=$(expr $col + 1)           #counts the number of cols

done < $one

#returns values
echo $col $row


}

#################################################################
#Name: Transpose
#Purpose: Return transpose of a matrix
#Input: A single matrix
#Output: The transpose 
#Description: Takes in a matrix and returns the transpose of the matrix
##################################################################
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

#loops through each 
for i in $(seq 1 $row)
do
#cats the file, cuts out each column, pastes the new row
cat  $1 | cut -f$i $1 | paste -s
done

else
#   Error with the file
    echo  "ERROR: File not readable" 1>&2
    exit 1 
fi 

}


#################################################################
#Name: Mean 
#Purpose: Return mean for each row 
#Input: A single matrix
#Output: The mean for each row in a single line
#Description: Takes in a matrix and returns the mean for each row
#             in a single line
##################################################################

mean()
{

#variables
row=0
col=0
tot=0
sum=0
num=0
mean=0
i=1

#files
temp="temp"
tempFile="tempFile"
final="final"
reversed="reversed"
revRemoved="revRemoved"


#reads in the file and counts num of rows and cols
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows
    col=$(expr $col + 1)           #counts the number of cols
    echo -n $myLine |  tr -d " " >> $temp
done <$1

#removes each col and stdout into tempFile ina single row
for i in $(seq 1 $row)
do
cat  $temp | cut -f$i $1 | paste -s >> $tempFile

done

#Reads tempFile and finds sum for each row
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

#final output
cat $final 

#removes all temp files
rm -f $temp
rm -f $final
rm -f $revRemoved
rm -f $reversed 
rm -f $tempFile

}

#################################################################
#Name: Add
#Purpose: Adds two matricies
#Input: Two matricies
#Output: The sum of both matricies
#Description: Takes two matricies and finds the sum
#             The matricies are validated according to addition rules
#             A matrix is returned with the sums
##################################################################
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

#Zero arguments returns error
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
#places sums in sumArray 
while read mat1 && read mat2 <&3
do 
   
    sum=$(expr $mat1 + $mat2 )
    #sum=$(expr $( cut -c $j $temp ) + $(cut -c $(expr $tot + $j ) $temp )) 
 
    IFS=$'\r\n' GLOBIGNORE='*' command eval  'sumArray+=($sum)'
    
done <$one 3<$two


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

#Removes all temp files
rm -f "sumTotals"
rm -f "temp1"
rm -f "temp2"
rm -f "one"
rm -f "two"
}

#################################################################
#Name: Multiply 
#Purpose: Multiplies two matricies
#Input: Two matricies
#Output: The product of both matricies
#Description: Takes two matricies and finds the products
#             The matricies are validated according to multiplication rules
#             A matrix is returned with the products
##################################################################
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
#puts file into temp2
while IFS= read -r -a n || [ -n "$n" ]
do  
    row2=$( head -n 1 $2 | wc -w )   #number of rows
    col2=$(expr $col2 + 1 )           #counts the number of cols
    echo "$n" >> $temp2
done <$2

#this runs if multiplication is possible 
if [ $row1 -eq $col2 ]
then

#loops through ind for col of first file
for i in $(seq 1 $(expr $col1 ) )
    do 
    #each line moved into tempfile
    cat $temp | tr " " "\t" | head -$i | tail -1 > tempfile

    #second file rows looped through
    for j  in $(seq 1 $(expr $row2 ) )

    do 
        count=1 
        sum=0
        cut -f $j $temp2  > tempfile2

        #loops through tempfile2
        #multiplies each and used that for the sum 
        while IFS='\t' read -r l || [ -n "$l" ]
        do 
            mul=$(($(cut -f $count tempfile )*l))
            sum=$(expr $sum + $mul )
            count=$(expr $count + 1 )

        done < $tempfile2

        #outputs products in new matrix
        if [ $j -lt $row2 ]
        then 
            echo -e -n "$sum\t"
        else
            echo $sum

        fi 

    done 
done 
        
else
    #the two matricies are not able to be multiplied
    echo "Error: The two matricies are not able to be multiplied"
    exit 1 
fi 


#Removes all temp files
rm -f $temp
rm -f $temp2
rm -f $tempfile
rm -f $tempfile2


}


#______________________Calling dims______________________
if [ $1 = 'dims' ]
then
    #greater than two arguments
    if (( $# > 2 ))
    then 
     echo "ERROR: Only one matrix for DIMS function" 1>&2
     exit 1 
    fi 

    #zero arguments
    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least one matrix" 1>&2
    exit 1
    fi 

    #function runs
    #used for cat piping
    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    #error with the file
    if [ ! -f $2 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    #function runs 
    #second argument saved at one
    if [ $# -gt 1 ]
    then 
    one=$2
    fi 

    #calling function
    dims $one

#____________________CALLING TRANSPOSE___________________
elif [ $1 = 'transpose' ]
then

    #greater than two arguments
     if (( $# > 2 ))
    then 
     echo "Error: Only one matrix for TRANSPOSE function" 1>&2
     exit 1 
    fi 

    #function runs
    #used for cat piping
    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    #function runs
    #saves second argument as one
    if [ $# -gt 1 ]
    then
    one=$2
    fi 

   #calling function 
    transpose $one



#___________________CALLING MEAN______________________

elif [ $1 = 'mean' ]
then

    #greater than two arguments
    if (( $# > 2 ))
    then 
     echo "ERROR: Only one matrix for MEAN function" 1>&2
     exit 1 
    fi 

    #zero arguments
    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least one matrix" 1>&2
    exit 1
    fi 

    #function runs
    #used for cat piping
    if [ $# -eq 1 ]
    then
    one=tempfile
    cat > $one 
    fi 

    #error with the file
    if [ ! -f $2 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    #function runs
    #saves second argument as one
    if [ $# -gt 1 ]
    then 
    one=$2
    fi 

    #calling function 
    mean $one
   
#________________________CALLING ADD_________________________
elif [ $1 = 'add' ]
then
    #greater than three arguments
    if (( $# > 3 ))
    then 
     echo "ERROR: Only two matrices for ADD function" 1>&2
     exit 1 
    fi 

    #error with the file
    if [ ! -f $2 ] || [ ! -f $3 ]
    then
        echo "Error: No such file" 1>&2
        exit 1  
    fi 
    
    #function rungs
    #saves two arguments as one and two
    if [ $# -gt 2 ]
    then 
    one=$2
    two=$3
    fi 

    #calling add
    add $one $two 


#_________________________CALLING MULTIPLY_________________________
elif [ $1 = 'multiply' ]
then
    #greater than three arguments
    if (( $# > 3 ))
    then 
     echo "ERROR: Only two matrices for MULTIPLY function" 1>&2
     exit 1 
    fi 

    #zero arguments
    if [ $# -eq 0 ]
    then 
    echo "ERROR: Need at least two matricies" 1>&2
    exit 1
    fi 

    #function runs
    #saves two arguments as one and two
    if [ $# -gt 2 ]
    then 
    one=$2
    two=$3
    fi 

    #calling multiply 
    multiply $one $two 

else
    #error for bad command
    echo "ERROR: Badcommand" 1>&2
    exit 1

fi 