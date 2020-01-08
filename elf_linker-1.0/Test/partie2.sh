#!/bin/bash
./testPartie2 $1 > test2
readelf -S $1 > test2readelf
diff -u -w -b -B test2 test2readelf
