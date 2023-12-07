

import argparse
import pdb
import sys
import os

sys.path.append(os.getcwd())
# print(sys.path)

from ss2hcsp.sl.sl_diagram import SL_Diagram
from ss2hcsp.sl.get_hcsp import get_hcsp
from ss2hcsp.hcsp.simulator import SimInfo, exec_parallel
from ss2hcsp.hcsp import hcsp
from ss2hcsp.hcsp import optimize


def printTofile(path, content, module=False):
    with open(path, "w") as f:
        if module:
            f.write("%type: module\n")
        f.write(content)

def print_module(path, m):
    with open(path, "w") as f:
        f.write("%type: module\n\n")
        f.write(m.export() + '\n')
        f.write("system\n  %s=%s()\nendsystem" % (m.name, m.name))

def start_trans(location,output_to_file=None):
    # First, parse and process diagram
    diagram = SL_Diagram(location=location)
    diagram.parse_xml()
    diagram.delete_subsystems()
    diagram.add_line_name()
    diagram.comp_inher_st()
    diagram.inherit_to_continuous()
    diagram.connect_goto()
    diagram.separate_diagram()

    

    # Convert to HCSP
    result_hp = get_hcsp(diagram)

    # Optimize module
    hp = result_hp.code
    procs = dict((proc.name, proc.hp) for proc in result_hp.procedures)
    procs, hp = optimize.full_optimize_module(procs, hp)
    result_hp.procedures = [hcsp.Procedure(name, hp) for name, hp in procs.items()]
    result_hp.code = hp

    if output_to_file:
        assert isinstance(output_to_file, str)
        print_module(output_to_file, result_hp)


if __name__ == "__main__":
    # pdb.set_trace()
    parser = argparse.ArgumentParser()
    parser.add_argument('-in','--inputfile',type=str, default = "Examples/Simulink/LunarLander.xml",help="path of inputfile")
    parser.add_argument('-out','--outputfile',type=str,default = "Examples/Simulink/LunarLander.txt",help= "output to certain path")
    args = parser.parse_args()

    start_trans(args.inputfile,output_to_file=args.outputfile)
