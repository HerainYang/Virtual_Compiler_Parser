#!/bin/bash

# Trigger all your test cases with this script
xargs -a tests/test_non_exist_input_file/mainAttri.in ./vm_x2017 | diff - tests/test_non_exist_input_file/correctoutput.out
xargs -a tests/test_no_ret/mainAttri.in ./vm_x2017 | diff - tests/test_no_ret/correctoutput.out
xargs -a tests/test_stack_overflow/mainAttri.in ./vm_x2017 | diff - tests/test_stack_overflow/correctoutput.out
xargs -a tests/test_wrong_argument/mainAttri.in ./vm_x2017 | diff - tests/test_wrong_argument/correctoutput.out
xargs -a tests/test_wrong_input_format/mainAttri.in ./vm_x2017 | diff - tests/test_wrong_input_format/correctoutput.out
