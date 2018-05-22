#!/bin/bash

test-library() {
    library=$1
    echo -n "Testing $library ... "
    if diff -q <(env LD_PRELOAD=./lib/$library ./bin/test_03) <($library-output); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library ./bin/test_03) <($library-output)
    fi
}

libmalloc-ff.so-output() {
    cat <<EOF
mallocs:   30
frees:     10
reuses:    18
grows:     12
splits:    12
coalesces: 0
blocks:    24
requested: 5124
max heap:  3584
EOF
}

libmalloc-nf.so-output() {
    cat <<EOF
mallocs:   30
frees:     10
reuses:    18
grows:     12
splits:    14
coalesces: 0
blocks:    26
requested: 5124
max heap:  3584
EOF
}

libmalloc-bf.so-output() {
    cat <<EOF
mallocs:   30
frees:     10
reuses:    18
grows:     12
splits:    11
coalesces: 2
blocks:    21
requested: 5124
max heap:  3328
EOF
}

libmalloc-wf.so-output() {
    cat <<EOF
mallocs:   30
frees:     10
reuses:    18
grows:     12
splits:    17
coalesces: 0
blocks:    29
requested: 5124
max heap:  3584
EOF
}

test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
