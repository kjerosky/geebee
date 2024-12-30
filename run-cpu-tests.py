import json
import os
import subprocess
import argparse
import sys
from tqdm import tqdm

def serialize_gameboy_state(gameboy_state: dict) -> str:
    serialized_state = str(gameboy_state['pc']) + ' '
    serialized_state += str(gameboy_state['sp']) + ' '
    serialized_state += str(gameboy_state['a']) + ' '
    serialized_state += str(gameboy_state['b']) + ' '
    serialized_state += str(gameboy_state['c']) + ' '
    serialized_state += str(gameboy_state['d']) + ' '
    serialized_state += str(gameboy_state['e']) + ' '
    serialized_state += str(gameboy_state['f']) + ' '
    serialized_state += str(gameboy_state['h']) + ' '
    serialized_state += str(gameboy_state['l']) + ' '

    ram_entries = gameboy_state['ram']
    serialized_state += str(len(ram_entries)) + ' '
    for ram_entry in ram_entries:
        serialized_state += str(ram_entry[0]) + ' '
        serialized_state += str(ram_entry[1]) + ' '

    return serialized_state

# ----------------------------------------------------------------------------

parser = argparse.ArgumentParser('run-cpu-tests')
parser.add_argument('-d', '--dir', help='The directory that contains the JSON tests.', type=str)
parser.add_argument('-t', '--test', help='A specific test to run.', type=str)

args = parser.parse_args()
dir = args.dir
test = args.test

test_files = []
if dir != None and test != None:
    sys.stderr.write('[ERROR] Please specify ONLY either a test directory or a specific test to run!\n')
    exit(1)
elif dir != None:
    if not os.path.exists(dir):
        sys.stderr.write('[ERROR] The specified test directory does not exist!\n')
        exit(1)
    elif not os.path.isdir(dir):
        sys.stderr.write('[ERROR] The specified test directory is not a directory!\n')
        exit(1)

    test_files = filter(lambda x: x.endswith('.json'), os.listdir(dir))
    test_files = list(map(lambda filename: os.path.join(dir, filename), test_files))
elif test != None:
    if not os.path.exists(test):
        sys.stderr.write('[ERROR] The specified test does not exist!\n')
        exit(1)
    elif not os.path.isfile(test):
        sys.stderr.write('[ERROR] The specified test is not a file!\n')
        exit(1)

    test_files = [test]
else:
    sys.stderr.write('[ERROR] Please specify either a test directory or a specific test to run!\n')
    exit(1)

test_files.sort()
test_count = 0
failed_test_count = 0
failed_test_names = []
failed_test_files = []
for test_file in test_files:
    with open(test_file) as file:
        base_test_file_name = os.path.basename(test_file)
        test_data = json.load(file)
        are_any_failed_tests = False
        for test in tqdm(test_data, desc=os.path.basename(base_test_file_name)):
            test_count += 1
            test_name = test['name']
            serialized_initial_state = serialize_gameboy_state(test['initial'])
            serialized_expected_final_state = serialize_gameboy_state(test['final'])
            test_input = serialized_initial_state.encode() + serialized_expected_final_state.encode()

            result = subprocess.run('./run-cpu-test', input=test_input)
            if result.returncode != 0:
                are_any_failed_tests = True
                failed_test_count += 1
                failed_test_names.append(test_name)

        if are_any_failed_tests:
            failed_test_files.append(base_test_file_name)

for failed_test_name in failed_test_names:
    print('Test ' + failed_test_name + ': [FAILED]')
if len(failed_test_files) > 0:
    print()
    print('Failures found in these files:')
    for failed_test_file in failed_test_files:
        print(failed_test_file)
print()
print('Total tests run: ' + str(test_count))
print('Tests passed: ' + str(test_count - failed_test_count))
print('Tests failed: ' + str(failed_test_count))
print()
print('FINAL RESULT: ' + ('✅' if len(failed_test_files) == 0 else '❌'))
print()
