#!/bin/bash

test-library() {
    library=$1
    echo -n "Testing $library ... "
    if diff -q <(env LD_PRELOAD=./lib/$library ./bin/test_01) <(test-output); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library ./bin/test_01) <(test-output)
    fi
}

test-output() {
    cat <<EOF
mallocs:   11
frees:     11
reuses:    10
grows:     1
splits:    10
coalesces: 10
blocks:    1
requested: 2052
max heap:  1024
EOF
}


test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
