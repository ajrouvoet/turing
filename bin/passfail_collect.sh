#!/bin/bash
if grep -q "PASS" ${TEST_LOG};
then
	echo "pass"
else
	echo "fail"
fi

exit 0
