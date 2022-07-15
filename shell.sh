#!/bin/sh

echo "1000000">freq.txt
timeout 60s ./b1
echo "100000">freq.txt
timeout 60s ./b1
echo "10000">freq.txt
timeout 60s ./b1
echo "1000">freq.txt
timeout 60s ./b1
echo "100">freq.txt
timeout 60s ./b1