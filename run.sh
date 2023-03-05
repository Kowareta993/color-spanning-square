#!/bin/bash

make

if [ $# -eq 0 ]
  then
    ./main    
  else
    ./main "$1" "$2"    
fi 
python plot.py


