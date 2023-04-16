
import sys
import os
import fnmatch
import toml

default_compile_flags = " -Wall" #-Werror"
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

def get_file_ending(file):
    parts = file.split('.')
    return parts.pop()

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

    with open(os.path.join(conf_dir, "Makefile"), 'w') as f:
        f.write("\n")
        f.write(".NOTPARALLEL:\n")
        f.write("\n")
        f.write(f"STD_CFLAGS={default_compile_flags}\n")
        f.write(f"COMPILE_FILE?=\n")
        f.write(f"EXE_FILE?=\n")
        f.write(f"TARGET=LOCAL")

        tasks = parsed_toml.items()
        for task_name, task_details in tasks:
            src_file = task_details.get('src', None);
            if (src_file == None):
                break;

            f.write(f"\n# ------ Configuration Values for {task_name} ({src_file}) ---------------------------------- #\n\n") 

            compiler = ""
            compiler_flags = ""
            if (get_file_ending(src_file) == "c"):
                compiler = "gcc"
                compiler_flags = "-std=c11"
            elif (get_file_ending(src_file) == "cpp"):
                compiler = "g++"

            links = [f"-{x}" for x in task_details.get("links", [])]
            opt = task_details.get("optimization_levels", ["0", "3", "fast"])
            profiler = task_details.get("profiler", "") 

            flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            comp_flags = [ f"-D{flag_name}=$({task_name}_{flag_name})" for flag_name, _ in flags ]
            env_vars = [ f"{env_name}=$({task_name}_{env_name})" for env_name, _ in envs ]

            f.write(f"{task_name}_FILE := {src_file}\n")
            f.write(f"{task_name}_EXE := {src_file}\n")
            f.write(f"{task_name}_LINKS := {' '.join(links)}\n")
            f.write(f"{task_name}_OPTIMIZATION_LEVEL ?= {f'-O{opt[0]}'}\n")
            f.write(f"{task_name}_OPTIMIZATION_LEVELS := {'-O' + ' -O'.join(opt)}\n")
            f.write(f"{task_name}_PROFILER ?= {profiler}\n")
            f.write(f"{task_name}_COMPILER ?= {compiler}\n")

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

            f.write(f"{task_name}_CFLAGS=$(STD_CFLAGS) {compiler_flags} $({task_name}_LINKS)\n")

        f.write(f"\n# ------ Run all Tasks Recipe ----- #\n\n") 
        f.write(f".PHONY: all\nall: {' '.join([f'{task}' for task in parsed_toml])}\n")

        for task_name, task_details in tasks:
            src_file = task_details.get('src', None);
            if (src_file == None):
                break;

            f.write(f"\n# ------ Recipes for {task_name} ({src_file}) ---------------------------------- #\n\n") 

            profiler = task_details.get("profiler", "") 
            opt = task_details.get('optimization_flags', ["0", "3", "fast"]);

            flags = task_details.get("Flags", {}).items()
            envs = task_details.get("Env", {}).items()

            comp_flags = [ f"-D{flag_name}=$({task_name}_{flag_name})" for flag_name, _ in flags ] 
            env_vars = [ f"{env_name}=$({task_name}_{env_name})" for env_name, _ in envs ]

            opt_foreach = f"@ $(foreach opt,$({task_name}_OPTIMIZATION_LEVELS), \\\n\t\tCOMPILE_FILE={src_file} \\\n\t\tEXE_FILE={remove_file_ending(src_file)} \\\n\t\t{task_name}_OPTIMIZATION_LEVEL=$(opt) \\\n\t\t{MAKE} run_{task_name}; \\\n\t)"

            loops = f"@ $(foreach opt,$({task_name}_OPTIMIZATION_LEVELS), \\\n\t\t"
            env_repl = []
            closing = " \\\n\t)"

            indent = 3
            for env_name, _ in envs:
                indent_str = '\t' * (indent - 1)
                loops += f"$(foreach {task_name}_{env_name}, $({task_name}_{env_name}_VALUES), \\\n\t{indent_str}" 
                env_repl.append(f"export {env_name}=$({task_name}_{env_name}) && ")
                closing = f" \\\n{indent_str})" + closing;
                indent += 1
            for flag, _ in flags:
                indent_str = '\t' * (indent - 1)
                loops += f"$(foreach {flag}, $({task_name}_{flag}_VALUES), \\\n\t{indent_str}"
                env_repl.append(f"{task_name}_{flag}=$({flag})")
                closing = f" \\\n{indent_str})" + closing;
                indent += 1

            env_repl.append(f"COMPILE_FILE={src_file}");
            env_repl.append(f"EXE_FILE={remove_file_ending(src_file)}");
            env_repl.append(f"OPTIMIZATION_LEVEL=$(opt)");

            f.write(f".PHONY: {task_name}\n")
            f.write(f"{task_name}:\n")
            indent -= 1
            break_str = " \\\n" + ("\t" * indent)
            env_flags = break_str.join(env_repl) + break_str
            f.write(f"\t{loops}{env_flags}{MAKE} run_{task_name};{closing}\n")
            f.write("\n")

            f.write(f".PHONY: run_{task_name}\nrun_{task_name}:\n")
            f.write(f"\t{MAKE} compile_{task_name}\n")
            f.write(f"ifneq ($(TARGET),LCC)\n")
            f.write(f"\t$({task_name}_PROFILER) ./$(EXE_FILE)\n")
            f.write(f"else\n")
            f.write(f"\tsbatch ../job.sh $(EXE_FILE)\n")
            f.write(f"endif\n\n")

            f.write(f".PHONY: {task_name}_OPT\n{task_name}_OPT: \n\t{opt_foreach}\n")
            f.write("\n")

            for flag, _ in flags:
                f.write(f".PHONY: {task_name}_{flag}\n{task_name}_{flag}: \n\t$(foreach val,$({task_name}_{flag}_VALUES), \\\n\t\tCOMPILE_FILE={src_file} \\\n\t\tEXE_FILE={remove_file_ending(src_file)} \\\n\t\t{task_name}_{flag}=$(val) \\\n\t\t{MAKE} run_{task_name}; \\\n\t)\n")
                f.write("\n")

            for env_name, _ in envs:
                f.write(f".PHONY: {task_name}_{env_name}\n{task_name}_{env_name}:\n\t$(foreach val,$({task_name}_{env_name}_VALUES), \\\n\t\tCOMPILE_FILE={src_file} \\\n\t\tEXE_FILE={remove_file_ending(src_file)} \\\n\t\t{task_name}_{env_name}=$(val) \\\n\t\t{MAKE} run_{task_name};\\\n\t)\n")
                f.write("\n")

            if (profiler == ""):
                profiler_flags = ""
            else:
                profiler_flags = "--shared -fPIC"

            f.write(f".PHONY: compile_{task_name}\ncompile_{task_name}:\n")
            f.write(f"\t$({task_name}_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $({task_name}_CFLAGS) {profiler_flags} $({task_name}_OPTIMIZATION_LEVEL) {' '.join(comp_flags)}\n\n")

