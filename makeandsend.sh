#!/bin/bash
make clean
make
adb push app /data/local/tmp
