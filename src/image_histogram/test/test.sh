#!/usr/bin/env bash
gcc -o histogram ../image_histogram.c

for f in *.png *.jpg
do
  diff <(./histogram "$f") "$f.histogram"
done

rm histogram
