import unittest
import importlib.util
import os

def execute_unit_tests(file_paths):
    suite = unittest.TestSuite()
    
    for file_path in file_paths:
        module_name = os.path.splitext(os.path.basename(file_path))[0]
        spec = importlib.util.spec_from_file_location(module_name, file_path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        
        test_loader = unittest.TestLoader()
        test_suite = test_loader.loadTestsFromModule(module)
        suite.addTests(test_suite)
        
    runner = unittest.TextTestRunner()
    result = runner.run(suite)
    
    return result

file_list = [
    'ss2hcsp/tests/expr_test.py',
    'ss2hcsp/tests/isabelle_test.py',
    'ss2hcsp/tests/matlab_test.py',
    'ss2hcsp/tests/module_test.py',
    'ss2hcsp/tests/optimize_test.py',
    'ss2hcsp/tests/parser_test.py',
    'ss2hcsp/tests/pprint_test.py',
    'ss2hcsp/tests/sf_convert_test.py',
    # 'ss2hcsp/tests/sf_isabelle_test.py',
    'ss2hcsp/tests/simulator_test.py',
    'ss2hcsp/tests/sim_test.py',
    # 'ss2hcsp/tests/systemC_test.py',
    'ss2hcsp/tests/topsort_test.py',
    'ss2hcsp/tests/transition_test.py',
    'aadl2hcsp/tests/aadl2json_test.py',
    'aadl2hcsp/tests/json2hcsp_test.py'
]

test_result = execute_unit_tests(file_list)
