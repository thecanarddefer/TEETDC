#!/bin/bash
./testPartie5 $1 > test5
readelf -r  $1 > test5readelf
diff -u -w -b -B test5 test5readelf
