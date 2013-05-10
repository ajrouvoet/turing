#!/bin/bash

if [ "$1" == '' ]
then
	echo "Test script run without argument";
	exit;
fi

cd $1 && make clean && make && ./main
