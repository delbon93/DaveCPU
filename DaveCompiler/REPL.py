import Pipeline
import InstructionCompiler as inscomp
import re
import struct

def write_to_binary_file(binary_data, file_name):
    if not file_name.endswith(".bin"):
        file_name += ".bin"
    with open(file_name, "wb") as file:
        for word in binary_data:
            file.write(word.to_bytes(2, byteorder="big"))

_exit_repl = False
_instruction_data = []

def _cmd_exit(args):
    global _exit_repl
    _exit_repl = True
    return

def _cmd_list(args):
    print(">> Listing <<")
    if len(args) > 0:
        if args[0] == "bin":
            for ins in _instruction_data:
                print(ins.construct())
                return
    for ins in _instruction_data:
        print(ins)
    print(">> ------- <<")
    return

def _cmd_out(args):
    if len(args) > 0:
        binary_data = []
        for ins in _instruction_data:
            binary_data += ins.construct()
        file_name = args[0]
        write_to_binary_file(binary_data, file_name)
        print("Saved %d words (%d bytes) to file '%s'" % (len(binary_data), len(binary_data) * 2, file_name))
    else:
        print("No file name given!")
    return

def _cmd_replace(args):
    if len(args) >= 2:
        label = args[0]
        replacement = args[1]
        for ins in _instruction_data:
            for parameter in ins.parameters:
                if inscomp.replace_label(parameter, label, replacement):
                    print(ins)
    else:
        print("Not enough parameters!")
    return

def _cmd_clear(args):
    global _instruction_data
    _instruction_data.clear()
    print("Cleared instruction list")
    return

def _cmd_pop(args):
    global _instruction_data
    pop_count = 1
    if len(args) > 0:
        pop_count = int(args[0])
    print("Popped %d instruction%s" % (pop_count, "" if pop_count == 1 else "s"))
    while pop_count > 0 and len(_instruction_data) > 0:
        pop_count -= 1
        _instruction_data.pop()
    return
  
_commands = {
    "exit quit q": _cmd_exit,
    "list": _cmd_list,
    "out save": _cmd_out,
    "replace subs": _cmd_replace,
    "clear": _cmd_clear,
    "pop": _cmd_pop
}

def _parse_instruction(line):
    global _instruction_data
    data = [d.lower() for d in Pipeline.tokenize(line)]
    parse_result = inscomp.parse_instruction(data)
    if not parse_result[1]:
        print(parse_result[2])
    else:
        print(parse_result[0])
        _instruction_data.append(parse_result[0])
    return

def start():
    print(" << INSTRUCTION PARSER >>")
    while not _exit_repl:
        user_input = input("$ ")
        input_tokens = user_input.split(" ")
        successful_command = False
        for key in _commands.keys():
            if input_tokens[0] in key.split(" "):
                _commands[key](input_tokens[1:])
                successful_command = True
        if not successful_command:
            try:
                _parse_instruction(user_input)
            except:
                print("Could not parse line: %s" % user_input)
    return        