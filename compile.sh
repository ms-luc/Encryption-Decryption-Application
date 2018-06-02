#!/bin/bash

if [ -z "$1" ]
  then
    echo "Executing ... no arguments"
    echo ""
    echo ""
    echo "output:"

    gcc -o my-cipher.o my-cipher.c -lm
    ./my-cipher.o

    exit
fi

echo "Executing ... $1 gcc: $2"
echo ""
echo ""
echo "output:"

gcc -o my-cipher.o my-cipher.c $2
./my-cipher.o $1

: '
for a
do
  if [ $a "-r" ]; then
    break;
  gcc -o my-cipher.o my-cipher.c $a
  fi
done

for b
do
  ./my-cipher.o $b
done
'
