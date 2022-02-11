#!/usr/bin/python3
#
#  Copyright Christopher Di Bella
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import subprocess
import sys
import argparse
import re
import os


def abort(process, message):
	print(f"Error: {process} {message}", file=sys.stderr)
	sys.exit(1)


def dump(name, data):
	with open(f'/tmp/{name}', 'w') as f:
		f.write(data)


def substitute_matches(process_name, expected, actual):
	placeholder = r'0x[\da-f]{16}'
	for i in re.findall(placeholder, actual):
		expected = expected.replace(placeholder, i, 1)

	placeholder = r'\+0x[\da-f]+'
	for i in re.findall(placeholder, actual):
		expected = expected.replace(placeholder, i, 1)

	process_name = re.search('.*(fail-(expects|assert|ensures)).*',
							 process_name).group(1)
	for i in re.findall(f'[n)] .+/test/{process_name}.cpp', actual):
		expected = expected.replace(f'.+/test/{process_name}.cpp', i[2:], 1)

	for i in re.findall(r'\(.*\+', actual):
		expected = expected.replace('(.*+', i, 1)

	for i in re.findall(f'_start \\(.+/test/{process_name}', actual):
		expected = expected.replace(f'_start (.+/test/{process_name}', i, 1)

	actual_columns = re.finditer(r'(.cpp:\d+:)(\d+)', actual)
	expected_columns = re.finditer(r'(.cpp:\d+:)(.+\+)', expected)

	for (ai, ei) in zip(actual_columns, expected_columns):
		expected = expected.replace(f'{ei.group(1)}{ei.group(2)}',
									f'{ei.group(1)}{ai.group(2)}')

	return expected


def check_program(args):
	process = f"./test/{args.process_name}"
	process_results = subprocess.run(process, capture_output=True, timeout=5)

	if process_results.returncode == 0:
		abort(process, "has unexpectedly succeeded.")

	if process_results.stdout:
		abort(process, "should not be writing to stdout.")

	if process_results.stderr and not args.debug:
		abort(process, f"wrote to stderr when building in a release mode:\n\n{process_results.stderr.decode()}\n")

	expected_result = 255
	if not args.debug and process_results.returncode != expected_result:
		abort(process, f"returned {process_results.returncode} when it should have returned {expected_result}")

	if args.debug:
		if not process_results.stderr:
			abort(process, "did not write to stderr while debugging symbols are present.")

		actual_output = process_results.stderr.decode()
		with open(args.expected_output, 'r') as f:
			expected_output = f.read()

		macros = {
			'equal_to': ['==', '0', '26'],
			'not_equal_to': ['!=', '1', '29'],
			'less': ['<', '0', '32'],
			'less_equal': ['<=', '0', '35'],
			'greater_equal': ['>=', '4', '38'],
			'greater': ['>', '4', '41']
		}

		index = re.sub('fail-(expects|assert|ensures)-', '', args.process_name)
		expected_output = expected_output.replace('%', macros[index][0], 1)
		expected_output = expected_output.replace('%', macros[index][1], 1)
		expected_output = expected_output.replace('%', macros[index][2], 1)
		expected_output = expected_output.replace('%', index, 1)

		if not re.match(expected_output, actual_output):
			expected_output = expected_output.replace('\(', '(')
			expected_output = expected_output.replace('\)', ')')
			expected_output = substitute_matches(
				args.process_name, expected_output, actual_output)
			dump(f'expected.{os.getpid()}', expected_output)
			dump(f'actual.{os.getpid()}', actual_output)
			subprocess.run(['diff', f'/tmp/expected.{os.getpid()}', f'/tmp/actual.{os.getpid()}'])
			abort(process, f'\n\nexpected output does not match actual output')


def parse_args():
	parser = argparse.ArgumentParser(description='Checks the output of a cjdb-contracts program.')
	parser.add_argument('--process-name', type=str, help='Process name as compiled by CMake.')
	parser.add_argument('--debug', type=bool, default=False, help='Determines if the process was compiled with debugging symbols.')
	parser.add_argument('--expected-output', type=str, help='Expected output for the process.')
	return parser.parse_args()


def main():
	args = parse_args()
	check_program(args)


if __name__ == "__main__":
	main()
