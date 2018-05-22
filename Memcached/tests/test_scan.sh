#!/bin/bash

scan-fifo-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 0
Misses    : 10240
EOF
}

scan-random-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 192
Misses    : 10048
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing scan with FIFO ... "
if diff -u <(./bin/scan 8 64 0 1) <(scan-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing scan with Random ... "
if diff -u <(./bin/scan 8 64 1 1) <(scan-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing scan with LRU ... "
if diff -u <(./bin/scan 8 64 2 1) <(scan-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing scan with Clock ... "
if diff -u <(./bin/scan 8 64 3 1) <(scan-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

