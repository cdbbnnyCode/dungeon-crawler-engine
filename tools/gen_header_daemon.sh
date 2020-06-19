#!/bin/bash

if [[ ${PWD##*/} = "tools" ]]; then
  cd ..
fi

inotifywait -mr -e create --include '.*\.h' src/ | (
  while read line; do
    a=( $line )
    dir=${a[0]}
    events=( $(echo ${a[1]} | tr ',' ' ') )
    fname=${a[2]}
    echo $dir$fname
    echo ${events[@]}
    if [[ " ${events[@]} " =~ " ISDIR " ]]; then
      echo "ignore directory"
      continue
    fi
    tools/gen_header.py $dir$fname
  done
)
