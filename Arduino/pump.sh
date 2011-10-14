#!/bin/sh

# Push a set of raw bytes to a serial terminal

TTY=/dev/tty.usbmodem411

if [ ! -f ./echoBytes ] 
    then
    gcc echoBytes.c -o echoBytes
fi

./echoBytes $* > $TTY

