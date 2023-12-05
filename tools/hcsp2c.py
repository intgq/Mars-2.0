import argparse
import subprocess

from hcsp2c import transfer2c
from ss2hcsp.hcsp.parser import parse_module_file

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-startfile',type=str,default = "./Examples/AADL/CCS/TCS/generatedcode_nobus/systemv2.txt")
    parser.add_argument('-outputfile',type=str,default = "systemv2")
    args = parser.parse_args()

    with open(args.startfile, "r") as f:
        s = f.read()
        infos = parse_module_file(s)

    (res, head_res, threads_res) = transfer2c.convertHps(args.outputfile, infos, step_size=1e-4, output_step_size=0.01,
                                max_time=40.0)

    run_str = "gcc"
    with open('hcsp2c/target/%s.c' % args.outputfile, 'w') as f:
        f.write(res)
        run_str += ' hcsp2c/target/%s.c' % args.outputfile
    with open('hcsp2c/target/%s.h' % args.outputfile, 'w') as f:
        f.write(head_res)
    for (thread_name, thread_body) in threads_res:
        with open('hcsp2c/target/%s__%s.c' % (args.outputfile, thread_name), 'w') as f:
            f.write(thread_body)
            run_str += ' hcsp2c/target/%s__%s.c' % (args.outputfile, thread_name)
    
    run_str += " hcsp2c/target/hcsp2c.c -lpthread -lm -o hcsp2c/output/%s.out -g" % args.outputfile

    res = subprocess.run(
        run_str,
        stderr=subprocess.PIPE,
        text=True,
        shell=True
    )

    # Handle exception resulting from conversion step
    if res.stderr != '':
        err = res.stderr
        raise Exception(err)

    res = subprocess.run(
        "hcsp2c/output/%s.out" % args.outputfile,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        shell=True
    )
