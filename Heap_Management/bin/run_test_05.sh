#!/bin/bash

test-library() {
    library=$1
    shift
    command=$@
    echo -n "Testing $library ($command)... "
    if env LD_PRELOAD=./lib/$library $command > /dev/null 2>&1; then
    	echo success
    else
    	echo failure
    fi
}

test-library libmalloc-ff.so cat README.md
test-library libmalloc-nf.so cat README.md
test-library libmalloc-bf.so cat README.md
test-library libmalloc-wf.so cat README.md

test-library libmalloc-ff.so md5sum README.md
test-library libmalloc-nf.so md5sum README.md
test-library libmalloc-bf.so md5sum README.md
test-library libmalloc-wf.so md5sum README.md

test-library libmalloc-ff.so du /lib
test-library libmalloc-nf.so du /lib
test-library libmalloc-bf.so du /lib
test-library libmalloc-wf.so du /lib

test-library libmalloc-ff.so find /lib
test-library libmalloc-nf.so find /lib
test-library libmalloc-bf.so find /lib
test-library libmalloc-wf.so find /lib
