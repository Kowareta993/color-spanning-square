#!/bin/bash

make
if [ ! -d "result" ]
then
    mkdir ./result
fi
if [ ! -d "analysis" ]
then
    mkdir ./analysis
fi
if [ $# -eq 0 ]
  then
    ./main    
  else
    ./main "$1" "$2"    
fi 
python plot.py


