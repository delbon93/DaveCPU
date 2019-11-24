
op_codes = {
    "lta": 0x01, "ltr": 0x02, "stm": 0x03, "str": 0x04,
    "add": 0x10, "adc": 0x11, "sub": 0x12, "mul": 0x13,
    "div": 0x14, "mod": 0x15, "inc": 0x16, "dec": 0x17,
    "cmp": 0x20, "and": 0x30, "orr": 0x31, "not": 0x32,
    "xor": 0x33, "nnd": 0x34, "nor": 0x35, "rol": 0x36,
    "ror": 0x37, "jmp": 0x40, "jeq": 0x41, "jne": 0x42,
    "jgt": 0x43, "jge": 0x44, "jlt": 0x45, "jle": 0x46,
    "rel": 0xF0, "csr": 0xF1, "rtn": 0xFF, "skp": 0x00
}

register_codes = {
    "sf": 0x0, "pc": 0x1, "acc": 0x2, "rel": 0x3, "fet": 0x4, 
    "ins": 0x5, "p1": 0x6, "p2": 0x7, "ret": 0x8, "sfo": 0x9, 
    "am": 0xA, "i0": 0xB, "i1": 0xC, "i2": 0xD, "i3": 0xE,
    "r0": 0x10, "r1": 0x11, "r2": 0x12, "r3": 0x13,
    "r4": 0x14, "r5": 0x15, "r6": 0x16, "r7": 0x17,
    "r8": 0x18, "r9": 0x19, "ra": 0x1A, "rb": 0x1B,
    "rc": 0x1C, "rd": 0x1D, "re": 0x1E, "rf": 0x1F
}

def _is_immediate(value):
    if value.startswith("0x") or value.startswith("0b"):
        value = value[2:]
    return value.isdigit()

def _get_numeric_value(immediate):
    if immediate.startswith("0x"):
        return int(immediate[2:], base=16)
    elif immediate.startswith("0b"):
        return int(immediate[2:], base=2)
    else:
        return int(immediate, base=10)

def _get_parameter_type_and_value(parameter):
    if parameter in register_codes.keys():
        return ("register", register_codes[parameter])
    elif parameter.startswith("$"):
        return ("abs_address", _get_numeric_value(parameter[1:]))
    elif parameter.startswith("&"):
        return ("rel_address", _get_numeric_value(parameter[1:]))
    elif _is_immediate(parameter):
        return ("immediate", _get_numeric_value(parameter))
    else:
        return ("label", parameter)


def _get_address_mode(op_code, parameter):
    pass


def compile_instrucion(ins_tokens):
    # expecting the first token to be the instruction name
    ins = ins_tokens[0].lower()
    # expecting all other tokens to be parameters
    parameters = ins_tokens[1:]