#!/bin/bash

test-library() {
    library=$1
    echo -n "Testing $library ... "
    if diff -q <(env LD_PRELOAD=./lib/$library ./bin/test_00) <(test-output); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library ./bin/test_00) <(test-output)
    fi
}

test-output() {
    cat <<EOF
mallocs:   10
frees:     10
reuses:    9
grows:     1
splits:    0
coalesces: 0
blocks:    1
requested: 10240
max heap:  1024
EOF
}

test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
