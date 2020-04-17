#!/usr/bin/env bash
gcc -o ical_peek ../ical_peek.c

for f in *.ics
do
  diff <(./ical_peek "$f") "$f.peek"
done

rm ical_peek
