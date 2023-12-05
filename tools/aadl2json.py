
# Unit test for aadl2json

import argparse
import json
import pdb
import sys
import os

sys.path.append(os.getcwd())
# print(sys.path)

from aadl2hcsp.aadl2json import convert_AADL, CompactJSONEncoder

if __name__ == "__main__":
    # pdb.set_trace()
    parser = argparse.ArgumentParser()
    parser.add_argument('-d','--dir',type=str, default = "Examples/AADL/CCS/AADL",help="directory of startfile and configfile, they need to in same folder")
    parser.add_argument('-sf','--startfile',type=str,default = "joint_model_nobus.aadl",help= "name of startfile")
    parser.add_argument('-cf','--configfile',type=str,default = "config.json",help= "name of configfile")
    args = parser.parse_args()

    info = convert_AADL(args.dir, args.startfile, args.configfile)
    start_filename = args.startfile.split('.')[-2]
    output_path = args.dir +"\\" + start_filename + ".json"
    f = open(output_path, "w")
    f.write(json.dumps(info, separators=(',', ': '), indent=4, cls=CompactJSONEncoder))
    f.close()
