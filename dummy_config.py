import os
from dummy.config.defaults import *

SUITES = {
	'all': [
		'unit_tests/*'
	]
}

METRICS = {
	'pass/fail' : {
		'collector': './bin/passfail_collect.sh'
	},

	'coverage' : {
		'collector': './bin/coverage_collect.sh',
		'type': 'json'
	}
}

TEST_RUNNER = os.path.join( './', TESTS_DIR, 'run.sh' )
