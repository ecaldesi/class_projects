#!/bin/bash

test-library() {
    library=$1
    echo -n "Testing $library ... "
    if diff -q <(env LD_PRELOAD=./lib/$library ./bin/test_02) <(test-output); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library ./bin/test_02) <(test-output)
    fi
}

test-output() {
    cat <<EOF
mallocs:   6
frees:     6
reuses:    2
grows:     4
splits:    1
coalesces: 1
blocks:    4
requested: 6144
max heap:  4096
EOF
}


test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
