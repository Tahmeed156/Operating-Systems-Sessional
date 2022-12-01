#!/bin/bash

while read -r key || [ -n "$key" ]; do  
  mkdir -p "./output/$key"
  for f in `grep -rl $key classified_01/`; do
    cp $f "./output/$key"
  done
done < keywords.txt

exit 0
