#!/usr/bin/sh

mkdir -p bin

gcc src/*.c -I src/ -o bin/yield
