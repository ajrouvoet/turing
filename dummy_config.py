import os
import sys

from dummy.config.defaults import *
from dummy.collector import Collector
from dummy.collector.generic import PassFailCollector, CCoverageCollector

sys.path.append( "bin" )

SUITES = {
	'all': [
		'unit_tests/*'
	]
}

METRICS = {
	'pass/fail' : {
		'collector': PassFailCollector()
	},

	'coverage' : {
		'collector': CCoverageCollector()
	}
}

TEST_RUNNER = os.path.join( './', TESTS_DIR, 'run.sh' )
