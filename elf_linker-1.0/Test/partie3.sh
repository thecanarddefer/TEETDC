#!/bin/bash
./testPartie3 $1 $2 > test3
readelf -x $2 $1 > test3readelf
diff -u -w -b -B test3 test3readelf
