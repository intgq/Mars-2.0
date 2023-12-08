import argparse
import subprocess
import sys
import os

sys.path.append(os.getcwd())
# print(sys.path)

from hcsp2c import transfer2c
from ss2hcsp.hcsp.parser import parse_module_file

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-sf', '--sourcefile', type=str, default = "using/output_files/TCS/hcsp/systemv2.txt", help = "name of the source combined HCSP file")
    parser.add_argument('-dd', '--destdirectory', type=str, default = "using/output_files/TCS/Ccode", help = "name of the destination directory of the output C code files")
    parser.add_argument('-df', '--destfile', type=str, default = "systemv2", help = "name of the destination file")
    parser.add_argument('-step', '--stepsize', type=float, default = 1e-4, help = "size of the step in discretization")
    parser.add_argument('-mt', '--maxtime', type=float, default = 40.0, help = "the max time of the execution")

    args = parser.parse_args()

    with open(args.sourcefile, "r") as f:
        s = f.read()
        infos = parse_module_file(s)

    (res, head_res, threads_res) = transfer2c.convertHps(args.destfile, infos, step_size=args.stepsize, output_step_size=0.01, max_time=args.maxtime)

    run_str = "gcc"
    with open(args.destdirectory + '/%s.c' % args.destfile, 'w') as f:
        f.write(res)
        run_str += ' %s.c' % args.destfile
    with open(args.destdirectory + '/%s.h' % args.destfile, 'w') as f:
        f.write(head_res)
    for (thread_name, thread_body) in threads_res:
        with open(args.destdirectory + '/%s__%s.c' % (args.destfile, thread_name), 'w') as f:
            f.write(thread_body)
            run_str += ' %s__%s.c' % (args.destfile, thread_name)
    
    run_str += " hcsp2c.c -lpthread -lm -o %s.out -g" % args.destfile
    with open(args.destdirectory + '/compile.sh', 'w') as f:
        f.write(run_str)
