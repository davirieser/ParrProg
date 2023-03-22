
import os
import re
import subprocess
from tabulate import tabulate

all_files = os.listdir(".")
pattern = re.compile(".+\.c")
files = [file for file in all_files if pattern.match(file)] 
opt_lvls = [ "-O0", "-O1", "-O2", "-O3", "-Ofast", "-Os" ] # -Og
num_threads = 8

also_execute = True 

if __name__ == "__main__":
    print(f"Running Tests for Files: {files}")
    for f in files:
        print(f"Compiling {f}")
        for o in opt_lvls:
            print(f"\t Using Optimization Level: {o}")
            exe = f.split('.')[0] + "." + o.replace('-', '')
            os.system(f"gcc {f} -o {exe} {o} -fopenmp") 
    if also_execute:
        benchmarks = []
        for f in files:
            for o in opt_lvls:
                for nc in range(1, num_threads): 
                    exe = "./" + f.split('.')[0] + "." + o.replace('-', '')
                    print(f"Running {exe} using {nc} Cores")
                    env = {}
                    env["OMP_NUM_THREADS"] = f"{nc}"
                    result = subprocess.check_output(exe, env=env)
                    result_str = result.decode("utf-8")
                    time = result_str.split(' ')[3]
                    print(f"\tTime taken {time} sec")
                    benchmarks.append((f, o, nc, f"{time} sec"))
        print(tabulate(benchmarks, 
                       headers=["FileName", "Optimization Level", "Number of Threads", "Time"], 
                       tablefmt="simple_outline")
              )
        

