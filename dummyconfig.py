import os
import sys

ROOT = os.path.dirname( __file__ )
TEST_RUNNER = 'tests/run.sh'
PRE_RUN_HOOK = "cd src/; make clean; make coverage"

ENV = {
	'DWARV_OPTIONS': "--*.rulestat:output:%s/.tmp/rulestat.log" % ROOT,
	'SOMETHING': 'awesome'
}

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
		'collector': 'dummy.honeypot.PassFailCollector'
	},

	'coverage' : {
		'collector': 'dummy.honeypot.CCoverageCollector',
		'kwargs': {
			'srcdirs': [
				'src/'
			],
			'extract': [
				'*/src/turingparser*',
				'*/src/error*'
			],
			'remove': [
				'*/src/turingparser*'
			]
		}
	}
}

STATISTICS = {
	'passing': {
		'engine': 'dummy.honeypot.CountEngine',
		'kwargs': {
			'metric': 'pass/fail'
		}
	},
	'coverage': {
		'engine': 'dummy.honeypot.CCoverageOverviewEngine'
	}
}

TARGETS = {
	'xil-perf': {
		'ENV': dict(
			ENV,
			DWARV_OPTIONS="--*.rulestat:skip --bb:some_opt_cool",
		)
	},

	'xil-cov': {
		# etc
	},

	'alt-perf': {
		# etc
	},

	'alt-cov': {
		# etc
	}
}
