#!/bin/bash

time-library() {
    library=$1
    echo -n "Timing $library ... "
    { time env LD_PRELOAD=./lib/$library ./bin/test_04; } |& awk '/real/ { print $2 }'
}

time-library libmalloc-ff.so
time-library libmalloc-nf.so
time-library libmalloc-bf.so
time-library libmalloc-wf.so
