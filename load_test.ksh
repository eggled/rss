#!/bin/bash

i=0
while [ $i -lt 1000 ]
do
    wget -q -O - http://localhost:8000/ > /dev/null &
done

wait
