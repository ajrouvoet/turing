import os
from dummy.config.defaults import *

SUITES = {
	'all': [
		'unit_tests/*'
	]
}

TEST_RUNNER = os.path.join( './', TESTS_DIR, 'run.sh' )
