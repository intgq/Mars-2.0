import argparse
import json
import pdb
import sys
import os

sys.path.append(os.getcwd())
# print(sys.path)

from aadl2hcsp.json2hcsp import translate_aadl_from_json
from ss2hcsp.hcsp.hcsp import convert_infos_to_concrete_chs, has_all_concrete_chs
from ss2hcsp.hcsp import parser
from ss2hcsp.hcsp.pprint import pprint
from ss2hcsp.hcsp.module import HCSPDeclarations, HCSPModule, HCSPModuleInst, HCSPSystem


if __name__ == "__main__":
    pdb.set_trace()
    parser = argparse.ArgumentParser()
    parser.add_argument('-jf','--jsonfile',type=str,default = "Examples/AADL/CCS/AADL/joint_model_nobus.json",help="directory of json file")
    parser.add_argument('-od','--outputdir',type=str,default = "Examples/AADL/CCS/TCS/generatedcode_nobus",help="directory (folder) of output hcsp file")
    parser.add_argument('-cf','--configfile',type=str,default = "config.json",help="name of configfile")
    args = parser.parse_args()

    
    with open(args.jsonfile, 'r') as f:
        jsoninfo = json.load(f)

    translate_aadl_from_json(jsoninfo, args.outputdir)

    with open(args.outputdir+"system.txt", 'r') as f:
            text = f.read()

    infos = parser.parse_module_file(text)
    infos2 = convert_infos_to_concrete_chs(infos)
    assert has_all_concrete_chs(infos2)

    modules = []
    module_insts = []
    for info in infos2:
        modules.append(HCSPModule(info.name, info.hp, outputs=info.outputs, procedures=info.procedures))
        module_insts.append(HCSPModuleInst(info.name, info.name))

    decls = HCSPDeclarations(modules + [HCSPSystem(module_insts)])
    with open(args.outputdir+"systemv2.txt", 'w') as f:
        f.write(decls.export())