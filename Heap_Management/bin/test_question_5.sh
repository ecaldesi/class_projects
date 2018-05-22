#!/bin/bash

test-library() {
    library=$1
    
    # testing cat README.md
    echo "Testing $library with cat README.md ..."
    env LD_PRELOAD=./lib/$library cat README.md | tail -n 9

    # testing ls 
    echo "Testing $library with ls ..."
    env LD_PRELOAD=./lib/$library ls | tail -n 9
    
    # testing sort Makefile
    echo "Testing $library with sort ..."
    env LD_PRELOAD=./lib/$library sort Makefile | tail -n 9
    
    # testing du Makefile 
    echo "Testing $library with du ..."
    env LD_PRELOAD=./lib/$library du Makefile | tail -n 9

    # testing md5sum Makefile 
    echo "Testing $library with md5sum ..."
    env LD_PRELOAD=./lib/$library md5sum Makefile | tail -n 9

}
test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
