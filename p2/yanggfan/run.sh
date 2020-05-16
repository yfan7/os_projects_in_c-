#!/bin/bash

x=1
while [ $x -le 10000 ]
do
          ./test1
          ./test2
          ./test3
          ./test4
          ./test5
          ./test6
          ./test7
          ./test8
          ./test10
          ./test11
          ./test12
          ./test13
          ./test14
          ./test15
            x=$(( $x + 1 ))
    done
