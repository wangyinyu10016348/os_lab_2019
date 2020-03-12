#!/bin/bash


echo 'количество аргументов: ' $#

i=0
sum=0
for an_arg in "$@" ; do
sum=$(($sum + ${an_arg}))
i=$(($i+1))
done

echo 'среднее арифметическое: '
expr $sum / $i

exit 0