#!/bin/bash

if [ "$1" == '' ]
then
	echo "Test script run without argument";
	exit 1;
fi

cd $1 && make clean && make

if [ -e "main" ]
then
	./main
fi

exit $?
