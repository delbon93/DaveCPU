import re
import struct
import InstructionCompiler as inscomp

precomp_subs = {}
data_definitions = []

def precompile(file_text):
    #remove comments
    block_comments = re.findall(r"/\*.*\*/", file_text, re.DOTALL)
    line_comments = re.findall(r"//.*$", file_text, re.MULTILINE)
    for comment in block_comments:
        file_text = re.sub(re.escape(comment), " ", file_text, re.DOTALL)
    for comment in line_comments:
        file_text = re.sub(re.escape(comment), " ", file_text, re.MULTILINE)

    # extract string literals
    string_literals = re.findall(r"\".*\"", file_text)
    str_count = 0
    for str_lit in string_literals:
        str_key = "%STR_" + str(str_count) + "%"
        file_text = re.sub(re.escape(str_lit), str_key, file_text)
        precomp_subs[str_key] = str_lit[1:-1]
        str_count += 1
    return file_text

def resub(symbol):
    if symbol in precomp_subs.keys():
        return precomp_subs[symbol]
    else:
        return symbol

def tokenize(file_text):
    for c in ["[", "]", "{", "}", ";", ","]:
        file_text = file_text.replace(c, " %s " % c)
    file_text = re.sub(r"\s+", " ", file_text)
    tokens = [resub(t) for t in file_text.split(" ") if len(t) > 0]



    return tokens

def compile(token_list):
    pass


def validate(token_list):
    return True


def sequence(file_name):
    with open(file_name, "r") as file:
        text = file.read()

    text = precompile(text)
    print("$ PRECOMPILED FILE TEXT\n")
    print(text)

    print("\n$ PRECOMPILE SUBSTITUTIONS\n")
    print(precomp_subs)
    
    token_list = tokenize(text)
    print("\n$ TOKENIZED FILE TEXT\n")
    print(token_list)
    #token_list = tokenize(atom_list)

    #if validate(token_list):
    #    print("Validated :)")
    #else:
    #    print("Not validated :c")

def write_to_binary_file(binary_data, file_name):
    if not file_name.endswith(".bin"):
        file_name += ".bin"
    with open(file_name, "wb") as file:
        for word in binary_data:
            file.write(word.to_bytes(2, byteorder="big"))

def repl():
    user_input = ""
    print(" << INSTRUCTION PARSER >>")
    binary_data = []
    while user_input.lower() not in ["exit", "quit", "q"]:
        user_input = input("$ ")
        if user_input in ["exit", "quit", "q"]:
            pass
        elif user_input in ["list"]:
            print(["0x%4x" % w for w in binary_data])
        elif user_input.startswith("out"):
            file_name = user_input.split(" ")[1]
            write_to_binary_file(binary_data, file_name)
            print("Saved %d words (%d bytes) to file '%s'" % (len(binary_data), len(binary_data) * 2, file_name))
        else:
            data = [d.lower() for d in tokenize(user_input)]
            parse_result = inscomp.parse_instruction(data)
            if not parse_result[1]:
                print(parse_result[2])
            else:
                print(parse_result[0])
                print(parse_result[0].construct(True))
                binary_data  += parse_result[0].construct(False)

if __name__ == "__main__":
    #sequence("test.asm")
    repl()
