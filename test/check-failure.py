#!/usr/bin/python3
#
# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
import subprocess
import sys
import argparse
import re

def abort(process, message):
	print(f"Error: {process} {message}", file=sys.stderr)
	sys.exit(1)

def check_program(args):
	process = f"./test/{args.process_name}"
	process_results = subprocess.run(process, capture_output=True, timeout=5)

	if process_results.returncode == 0:
		abort(process, "has unexpectedly succeeded.")

	if process_results.stdout:
		abort(process, "should not be writing to stdout.")

	if process_results.stderr and not args.debug:
		abort(process, "wrote to stderr when building in a release mode.")

	expected_result = 255
	if not args.debug and process_results.returncode != expected_result:
		abort(process, f"returned {process_results.returncode} when it should have returned {expected_result}")

	if args.debug:
		if not process_results.stderr:
			abort(process, "did not write to stderr while debugging symbols are present.")

		actual_output = process_results.stderr.decode()
		match_result = re.match(args.expected_output, actual_output)
		if match_result:
			abort(process, f"\n\nexpected: {args.expected_output}\nactual:   {actual_output}")


def parse_args():
	parser = argparse.ArgumentParser(description='Checks the output of a cjdb-contracts program.')
	parser.add_argument('--process-name', type=str, help='Process name as compiled by CMake.')
	parser.add_argument('--debug', type=bool, default=False, help='Determines if the process was compiled with debugging symbols.')
	parser.add_argument('--expected-output', type=str, help='Expected output for the process.')
	return parser.parse_args()


def main():
	args = parse_args()
	check_program(args)


main()
