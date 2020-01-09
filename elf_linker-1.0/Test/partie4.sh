#!/bin/bash
./testPartie4 $1 > test4
readelf -s  $1 > test4readelf
diff -u -w -b -B test4 test4readelf
