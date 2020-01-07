#!/bin/bash
./testPartie1 $1 > test1
readelf -h $1 > test1readelf
diff -u -w -b -B test1 test1readelf