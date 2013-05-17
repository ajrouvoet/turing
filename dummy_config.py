import os
import sys

from dummy.config.defaults import *
from dummy.collector import Collector
from dummy.collector.generic import PassFailCollector, CCoverageCollector
from dummy.statistics.generic import CountEngine, CoverageOverviewEngine

sys.path.append( "bin" )

SUITES = {
	'all': [
		'unit_tests/*',
		'integration/*'
	],

	'integration': [
		'integration/*'
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

STATISTICS = {
	'tests passing': {
		'engine': CountEngine( metric='pass/fail' )
	},
	'coverage': {
		'engine': CoverageOverviewEngine()
	}
}

TEST_RUNNER = os.path.join( './', TESTS_DIR, 'run.sh' )
