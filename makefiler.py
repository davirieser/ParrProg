
import sys
import os
import fnmatch
import toml

default_compile_flags = " -std=c11 -Wall -Werror"
MAKE = "$(MAKE) -e --no-print-directory"

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

        tasks = parsed_toml.items()
        for task_name, task_details in tasks:
            src_file = task_details.get('src', None);
            if (src_file == None):
                break;

            links = [f"-{x}" for x in task_details.get("links", [])]
            opt = task_details.get("optimization_levels", ["0", "3", "fast"])
            profiler = task_details.get("profiler", "") 

            flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            comp_flags = [ f"-D{flag_name}=$({task_name}_{flag_name})" for flag_name, _ in flags ]
            env_vars = [ f"{env_name}=$({task_name}_{env_name})" for env_name, _ in envs ]

            f.write("\n")

            f.write(f"{task_name}_FILE := {src_file}\n")
            f.write(f"{task_name}_EXE := {src_file}\n")
            f.write(f"{task_name}_LINKS := {' '.join(links)}\n")
            f.write(f"{task_name}_OPTIMIZATION_LEVEL ?= {f'-O{opt[0]}'}\n")
            f.write(f"{task_name}_OPTIMIZATION_LEVELS := {'-O' + ' -O'.join(opt)}\n")
            f.write(f"{task_name}_PROFILER ?= {profiler}\n")

            if (len(comp_flags) > 0): 
                f.write("# Compiler Flags\n")
            for flag, values in flags:
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

        for task_name, task_details in tasks:
            src_file = task_details.get('src', None);
            if (src_file == None):
                break;

            profiler = task_details.get("profiler", "") 
            opt = task_details.get('optimization_flags', ["0", "3", "fast"]);

            flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            comp_flags = [ f"-D{flag_name}=$({task_name}_{flag_name})" for flag_name, _ in flags ] 
            env_vars = [ f"{env_name}=$({task_name}_{env_name})" for env_name, _ in envs ]

            opt_foreach = f"$(foreach opt,$({task_name}_OPTIMIZATION_LEVELS), COMPILE_FILE={src_file} EXE_FILE={remove_file_ending(src_file)} {task_name}_OPTIMIZATION_LEVEL=$(opt) {MAKE} run_{task_name};)"

            loops = f"$(foreach opt,$({task_name}_OPTIMIZATION_LEVELS), "
            env_repl = f"{task_name}_OPTIMIZATION_LEVEL=$(opt) COMPILE_FILE={src_file} EXE_FILE={remove_file_ending(src_file)} "
            closing = ")"

            for env_name, _ in envs:
                loops += f"$(foreach {env_name}, $({task_name}_{env_name}_VALUES),"
                env_repl = f"{task_name}_{env_name}=$({env_name}) {env_repl}";
                closing += ")";
            for flag, _ in flags:
                loops += f"$(foreach {flag}, $({task_name}_{flag}_VALUES),"
                env_repl = f"{task_name}_{flag}=$({flag}) {env_repl}";
                closing += ")";

            f.write(f".PHONY: {task_name}\n")
            f.write(f"{task_name}:\n")
            f.write(f"\t{loops} {env_repl} {MAKE} run_{task_name};{closing}\n")
            f.write("\n")

            f.write(f".PHONY: run_{task_name}\nrun_{task_name}:\n")
            f.write(f"\t{MAKE} compile_{task_name}\n")
            f.write(f"\t{' '.join(env_vars)} $({task_name}_PROFILER) ./$(EXE_FILE)\n")
            f.write("\n")

            f.write(f".PHONY: {task_name}_OPT\n{task_name}_OPT: \n\t@ {opt_foreach}\n")
            f.write("\n")

            for flag, _ in flags:
                f.write(f".PHONY: {task_name}_{flag}\n{task_name}_{flag}: \n\t$(foreach val,$({task_name}_{flag}_VALUES), COMPILE_FILE={src_file} EXE_FILE={remove_file_ending(src_file)} {task_name}_{flag}=$(val) {MAKE} run_{task_name};)\n")
                f.write("\n")

            for env_name, _ in envs:
                f.write(f".PHONY: {task_name}_{env_name}\n{task_name}_{env_name}:\n\t$(foreach val,$({task_name}_{env_name}_VALUES), COMPILE_FILE={src_file} EXE_FILE={remove_file_ending(src_file)} {task_name}_{env_name}=$(val) {MAKE} run_{task_name};)\n")
                f.write("\n")

            if (profiler == ""):
                profiler_flags = ""
            else:
                profiler_flags = "--shared -fPIC"

            f.write(f".PHONY: compile_{task_name}\ncompile_{task_name}:\n")
            f.write(f"\tgcc $(COMPILE_FILE) -o $(EXE_FILE) $({task_name}_CFLAGS) {profiler_flags} $({task_name}_OPTIMIZATION_LEVEL) {' '.join(comp_flags)}\n\n")

