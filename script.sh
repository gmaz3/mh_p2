#!/bin/bash

directorio="data"

for i in $(ls $directorio)
do
    echo $i
    ./bin/am-10-1.0 'data/'$i 531
done
