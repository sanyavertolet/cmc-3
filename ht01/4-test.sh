#!/bin/bash

let NTEST=0

debug()
{
    local str
    let NTEST=$NTEST+1
    while [ "$1" != "" ] ; do
        str="$str \"$1\""
        shift
    done
    echo "TEST $NTEST:"
    echo "$str"
    eval "./a.out $str"
}


debug libc.so.6 puts vs "hello! its me!"
debug libc.so.6 puts is "hello! its me!"
debug libc.so.6 printf vssidi "hello Meet %s %d %f %c" Abcd 123 11.1 0x0A
debug libc.so.6 printf isisidddddi "hello Meet \"%*s\" %d %f %f %f %f %f%c" 10 Abcd 123 11.1 22.2 33.3 44.4 55.5 10
debug libc.so.6 strcmp iss "hello, world!" "hell"
debug libc.so.6 strcmp iss "hello, world!" "lo? word"
debug libm.so.6 cos dd 0.55
debug libm.so.6 pow ddd 17 0.5
debug libm.so.6 pow ddd 4.12311 2
debug libc.so.6 rand iv

exit 0

./a.out libc.so.6 puts vs "hello! its me!"
./a.out libc.so.6 puts is "hello! its me!"
./a.out libc.so.6 printf vssidi "hello Meet %s %d %f %c" Abcd 123 11.1 0x0A
./a.out libc.so.6 printf isisidddddi "hello Meet \"%*s\" %d %f %f %f %f %f%c" 10 Abcd 123 11.1 22.2 33.3 44.4 55.5 10
./a.out libc.so.6 strcmp iss "hello, world!" "hell"
./a.out libc.so.6 strcmp iss "hello, world!" "lo? word"
./a.out libm.so.6 cos dd 0.55
./a.out libm.so.6 pow ddd 1024 0.5
./a.out libm.so.6 pow ddd 17 0.5
./a.out libc.so.6 rand iv
