
import sys
import os
import fnmatch
import toml

default_compile_flags = " -std=c11 -Wall -Werror --shared -fPIC"

# https://stackoverflow.com/a/1724723
# Returns first File matching <Pattern> in <Path>
def find(pattern, path):
    for root, dirs, files in os.walk(path):
        for name in files:
            if fnmatch.fnmatch(name, pattern):
                return os.path.join(root, name)
    return None

def remove_file_ending(file):
    parts = file.split('.')
    parts.pop()
    return ''.join(parts)

if __name__ == "__main__":
    if (len(sys.argv) != 2):
        print(f"Usage: {sys.argv[0]} <DIR>") 
        print(f"DIR: Needs to contain *.toml Configuration File")
        exit(1)

    conf_dir = sys.argv[1] 
    if (not os.path.exists(os.path.dirname(conf_dir))):
        print(f"<{conf_dir}> is not a Directory!")
        exit(1)

    conf_file = find('*.toml', conf_dir)
    if (conf_file == None):
        print("Could not find Config File")
        exit(1)


    with open(conf_file, 'r') as f:
        parsed_toml = toml.loads(f.read())

    print(parsed_toml)

    with open(os.path.join(conf_dir, "Makefile"), 'w') as f:
        f.write("\n")
        f.write(".NOTPARALLEL:\n")
        f.write("\n")
        f.write(f"STD_CFLAGS={default_compile_flags}\n")
        f.write(f"COMPILE_FILE?=\n")
        f.write(f"EXE_FILE?=\n")

        for task_name, task_details in parsed_toml.items():
            if (task_details["src"] == []):
                break;

            src_files = task_details['src'];
            links = [f"-{l}" for l in task_details.get('links', [])];
            threads = [str(x) for x in task_details.get('num_threads', [1, 8])];
            opt = task_details.get('optimization_flags', ["0", "3", "fast"]);
            profiler = task_details.get('profiler', "")

            comp_flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            f.write("\n")

            f.write(f"{task_name}_FILES := {' '.join(src_files)}\n")
            f.write(f"{task_name}_LINKS := {' '.join(links)}\n")
            f.write(f"{task_name}_THREADS := {' '.join(threads)}\n")
            f.write(f"{task_name}_OPTIMIZATION_LEVELS := {'O' + ' O'.join(opt)}\n")
            f.write(f"{task_name}_PROFILER := {profiler}\n")

            if (len(comp_flags) > 0): 
                f.write("# Compiler Flags\n")
            for flag, values in comp_flags:
                f.write(f"\t{task_name}_{flag} ?= {str(values[0])}\n")
                f.write(f"\t{task_name}_{flag}_VALUES = {' '.join([str(x) for x in values])}\n")

            if (len(comp_flags) > 0): 
                f.write("# Environment Variables\n")
            for env_var, values in envs:
                f.write(f"\t{task_name}_{env_var} ?= {str(values[0])}\n")
                f.write(f"\t{task_name}_{env_var}_VALUES = {' '.join([str(x) for x in values])}\n")

            f.write(f"{task_name}_CFLAGS=$(STD_CFLAGS) $({task_name}_LINKS)\n")

        f.write("\n")
        f.write(f".PHONY: all\nall: {' '.join([f'{task}' for task in parsed_toml])}\n\n")

        for task_name in parsed_toml:
            f.write(f".PHONY: {task_name}\n")
            f.write(f"{task_name}: \n")
            f.write("\n")

        for task_name, task_details in parsed_toml.items():
            if (task_details["src"] == []):
                break;

            flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            comp_flags = [ f"-D{flag_name}=$({task_name}_{flag_name})" for flag_name, _ in flags ] 
            env_vars = [ f"{env_name}=$({task_name}_{env_name})" for env_name, _ in envs ]

            f.write(f".PHONY: compile_{task_name}\ncompile_{task_name}:\n")
            f.write(f"\tgcc $(COMPILE_FILE) -o $(EXE_FILE) $({task_name}_CFLAGS) {' '.join(comp_flags)}\n\n")

            f.write(f".PHONY: run_{task_name}\nrun_{task_name}:\n")
            f.write(f"\t$(MAKE) -e compile_{task_name}\n")
            f.write(f"\t{' '.join(env_vars)} $({task_name}_PROFILER) ./$(EXE_FILE)\n")
            f.write("\n")

