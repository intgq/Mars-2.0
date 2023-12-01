"""Unit test for sf_convert."""

import unittest
import random
from pstats import Stats
import cProfile

from ss2hcsp.sf import sf_convert
from ss2hcsp.sl.sl_diagram import SL_Diagram
from ss2hcsp.hcsp import module
from ss2hcsp.hcsp import hcsp
from ss2hcsp.tests.simulator_test import run_test as run_simulator_test
from ss2hcsp.tests.sim_test import run_test as run_sim_test
from ss2hcsp.hcsp.pprint import pprint


def run_test(self, filename, num_cycle, res, *, io_filter=None,
             print_chart=False, print_before_simp=False, print_final=False,
             debug_name=False, print_res=False, profile=False, output_to_file=None):
    """Test function for Stateflow diagrams.

    filename : str - name of the XML file.
    num_cycle : int - number of cycles of Stateflow diagram to simulate.
    res : List[str] - expected output events.
    io_filter : str -> bool - (optional) filter for IO events to display.
    print_chart : bool - print parsed chart.
    print_before_simp : bool - print HCSP program before simplification.
    print_final : bool - print HCSP program after optimization.
    debug_name : bool - print size of HCSP program before and after optimization.
    output_to_file : str - (optional) name of file to output HCSP.

    """
    if profile:
        pr = cProfile.Profile()
        pr.enable()

    diagram = SL_Diagram(location=filename)
    proc_map = sf_convert.convert_diagram(
        diagram, print_chart=print_chart, print_before_simp=print_before_simp,
        print_final=print_final, debug_name=debug_name)

    if profile:
        p = Stats(pr)
        p.strip_dirs()
        p.sort_stats('cumtime')
        p.print_stats()

    # Optional: output converted HCSP to file
    if output_to_file is not None:
        modules = []
        module_insts = []
        for name, (procs, hp) in proc_map.items():
            procs_lst = [hcsp.Procedure(proc_name, hp)
                         for proc_name, hp in procs.items()]
            modules.append(module.HCSPModule(
                name, code=hp, procedures=procs_lst))
            module_insts.append(module.HCSPModuleInst(name, name, []))
        system = module.HCSPSystem(module_insts)
        declarations = module.HCSPDeclarations(modules + [system])

        with open(output_to_file, "w") as f:
            f.write(declarations.export())

    # Test result using simulator
    run_simulator_test(self, proc_map, num_cycle, res, io_filter=io_filter,
                       print_res=print_res)


# Path to all test cases
prefix = "./Examples/Stateflow/tests/"


class SFConvertTest(unittest.TestCase):
    # def testStates1(self):
    #     run_test(self, prefix+"States/States1.xml", 3,
    #              ['log enA', 'log enA1', 'log duA', 'log exA1', 'log enA2', 'delay 0.1',
    #               'log duA', 'log duA2', 'delay 0.1', 'log duA', 'log duA2', 'delay 0.1'],
    #              output_to_file=prefix+"States/States1.txt")

    def testYoyoControl(self):
        run_test(self, "./Examples/Simulink/sf_yoyo_2018a.xml", 3,
                 ['delay 100', 'delay 100', 'delay 100'],
                 output_to_file="./Examples/Simulink/YoyoControl.txt")
    
    # def testseldemo(self):
    #     run_test(self, "./Examples/Simulink/sldemo_autotrans_2018a.xml", 3,
    #              ['delay 100', 'delay 100', 'delay 100'],
    #              output_to_file="./Examples/Simulink/sldemo_autotrans.txt")

    # def testcontinuousPID(self):
    #     run_test(self, "./Examples/Simulink/continuousPID_2018a.xml", 3,
    #              ['delay 100', 'delay 100', 'delay 100'],
    #              output_to_file="./Examples/Simulink/continuousPID.txt")


if __name__ == "__main__":
    unittest.main()
