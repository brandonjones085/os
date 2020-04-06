#!/bin/bash

row=0
i=1

#This is mostly to just get the number of rows
#by counting the first row
while IFS= read -r myLine || [ -n "$myLine" ]
do  
    row=$( head -n 1 $1 | wc -w )   #number of rows

done < $1

for i in $(seq 1 $row)
do
cat  $1 | cut -f$i $1 | paste -s 
done
