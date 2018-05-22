#!/bin/bash

test-library() {
    library=$1
    
    # testing cat README.md
    echo -n "Testing $library with cat README.md ..."
    if diff -q <(env LD_PRELOAD=./lib/$library cat README.md | head -n -9) <(test-output-cat); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library cat README.md | head -n -9) <(test-output-cat)
    fi

    # testing ls 
    echo -n "Testing $library with ls ..."
    if diff -q <(env LD_PRELOAD=./lib/$library ls | head -n -9) <(test-output-ls); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library ls | head -n -9) <(test-output-ls)
    fi
    
    # testing sort 
    echo -n "Testing $library with sort ..."
    if diff -q <(env LD_PRELOAD=./lib/$library sort Makefile | head -n -9) <(test-output-sort); then
    	echo "success"
    else
    	echo "failure"
	diff -u <(env LD_PRELOAD=./lib/$library sort Makefile | head -n -9) <(test-output-sort)
    fi
    
}

test-output-cat() {
    cat README.md
}

test-output-ls() {
    ls
}

test-output-sort() {
    sort Makefile 
}

test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so

