#!/bin/bash

while read p; do

ffmpeg -i "$p" 2>&1 | grep Dur | cut -d: -f2,3,4| cut -d. -f1 | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }' >> /tmp/musicpv.file.dur

done < "/tmp/musicpv.file"

rm /tmp/musicpv.file
