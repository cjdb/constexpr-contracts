#!/usr/bin/python
# Copyright (c) Google LLC.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
import sys
import re
import argparse
import subprocess


def parse_args():
    parser = argparse.ArgumentParser(
        description='Checks the program output matches the expected output.')
    parser.add_argument('--expected-output',
                        type=str,
                        help='Path to the expected program output.')
    parser.add_argument('--process-name',
                        type=str,
                        help='Path to the actual program to check.')
    return parser.parse_args()


def read_from_file(path):
    with open(path, 'r') as f:
        return f.read()


def run_program(process_name):
    process_results = subprocess.run(process_name,
                                     capture_output=True,
                                     timeout=5)

    if process_results.returncode != 0:
        print(
            f"{process_name} unexpectedly failed with code {process_results.returncode}",
            file=sys.stderr)
        sys.exit(1)

    if process_results.stdout:
        print(f"{process_name} shouldn't be writing to stdout.",
              file=sys.stderr)
        sys.exit(1)

    if not process_results.stderr:
        print(
            f"{process_name} should've written something to stderr, but didn't.",
            file=sys.stderr)
        sys.exit(1)

    return process_results.stderr.decode()


def escape(data):
    data = data.replace('[', '\\[')
    data = data.replace(']', '\\]')
    data = data.replace('*', '\\*')
    data = data.replace('+', '\\+')
    data = data.replace('(', '\\(')
    data = data.replace(')', '\\)')
    data = data.replace('.', '\\.')
    data = data.replace(r'0x\[\da-f\]\+', r'0x[\da-f]+')
    return data


def unescape(data):
    data = data.replace('\\[', '[')
    data = data.replace('\\]', ']')
    data = data.replace('\\*', '*')
    data = data.replace('\\+', '+')
    data = data.replace('\\(', '(')
    data = data.replace('\\)', ')')
    data = data.replace('\\.', '.')
    return data


def main():
    args = parse_args()
    expected = escape(read_from_file(args.expected_output)).split('\n')
    actual = run_program(args.process_name).split('\n')
    mismatch = False

    if len(expected) != len(actual):
        print("Mismatch between array sizes.", file=sys.stderr)
        sys.exit(1)

    for i, j, line in zip(expected, actual, range(len(expected))):
        if not re.match(i, j):
            mismatch = True
            print(f"Line {line + 1}:", file=sys.stderr)
            print(f"\tExpected:\t{unescape(i)}", file=sys.stderr)
            print(f"\tActual:  \t{j}", file=sys.stderr)
    sys.exit(int(mismatch))


if __name__ == "__main__":
    main()
