#!/bin/bash

output() {
    cat <<EOF
Addresses : 16
Pages     : 4
VPNShift  : 2
VPNMask   : 0xC
OffsetMask: 0x3
Hits      : 0
Misses    : 20
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Functional test with FIFO ... "
if diff -u <(./bin/test 8 64 0 1) <(output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Functional test with Random ... "
if diff -u <(./bin/test 8 64 1 1) <(output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
echo -n "Functional test with LRU ... "
if diff -u <(./bin/test 8 64 2 1) <(output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
echo -n "Functional test with Clock ... "
if diff -u <(./bin/test 8 64 3 1) <(output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
