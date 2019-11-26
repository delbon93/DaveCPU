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


class Parameter:

    def __init__(self, word, type, am=0, label="", is_label=False):
        self.word = word
        self.label = label
        self.type = type
        self.needs_label_subsitution = is_label
        self.address_mode = am

    def __str__(self):
        if self.needs_label_subsitution:
            return "Label = %s, Type = %s [requires label substitution]" % (self.label, self.type)
        else:
            return "Word = 0x%x, Type = %s" % (self.word, self.type)

class Instruction:

    def __init__(self, op_code):
        self.op_code = op_code
        self.parameters = []

    def _am(self, i):
        if len(self.parameters) >= i:
            return self.parameters[i-1].address_mode
        else:
            return 0

    def construct(self, as_string=False):
        ins = self.op_code * 256
        ins += self._am(1) * 32
        ins += self._am(2) * 4
        ins += len(self.parameters)
        ins = "%x" % ins
        ins = int("%s%s" % ("0" * (4 - len(ins)), ins), 16)

        data = [ins]
        for parameter in self.parameters:
            data.append(parameter.word)
            

        if as_string:
            return ["0x%4x" % w for w in data]
        else:
            return data

    def length(self):
        return 1 + len(self.parameters)

    def __str__(self):
        name = "undefined"
        for key in op_codes.keys():
            if op_codes[key] == self.op_code:
                name = key.upper()
                break

        str = "OpCode=0x%x (%s), Address Modes = %d | %d" % (self.op_code, name, self._am(1), self._am(2))
        i = 1
        for parameter in self.parameters:
            str += "\n%d. Parameter: %s" % (i, parameter)
            i += 1
        return str


def _is_immediate(value):
    if value.startswith("0x") or value.startswith("0b") or value == "0":
        return True
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


def _get_address_mode(op_code, parameter_number, parameter_type):
    if op_code == 0x01:
        return {"immediate": 1, "abs_address": 2, "rel_address": 3, "register": 4}[parameter_type]
    elif op_code == 0x02:
        if parameter_number == 1:
            return {"register": 1}[parameter_type]
        else:
            return {"immediate": 1, "abs_address": 2, "rel_address": 3, "register": 4}[parameter_type]
    elif op_code == 0x03:
        if parameter_number == 1:
            return {"abs_address": 1, "rel_address": 2}[parameter_type]
        else:
            return {"immediate": 1, "register": 2}[parameter_type]
    elif op_code in [0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x20, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37]:
        return {"immediate": 1, "register": 2}[parameter_type] 
    elif op_code in [0x04, 0x16, 0x17]:
        return {"register": 1}[parameter_type] 
    elif op_code in range(0x40, 0x47):
        return 1
    elif op_code in [0xF0, 0xF1]:
        return {"abs_address": 1}[parameter_type]
    else:
        return 0

def parse_instruction(ins_tokens):
    success = True
    error = ""

    # expecting the first token to be the instruction name
    ins = ins_tokens[0]
    op_code = op_codes[ins]
    instruction_object = Instruction(op_code)
    # expecting all other tokens to be parameters
    parameter_list = ins_tokens[1:]
    parameter_count = 0
    address_modes = [0, 0]

    for p in parameter_list:
        parameter_count += 1

        parameter_type, parameter_value = _get_parameter_type_and_value(p)
        parameter_object = Parameter(parameter_value, parameter_type)
        
        

        if parameter_type == "label":
            parameter_object.label = parameter_value
            parameter_object.word = 0
            parameter_object.needs_label_subsitution = True
        else:
            try:
                parameter_object.address_mode = _get_address_mode(op_code, parameter_count, parameter_type)
            except:
                print("Invalid address mode:\nop_code=%d, p_count=%d, p_type=%s" % (op_code, parameter_count, parameter_type))
                parameter_object.address_mode = 0

        instruction_object.parameters.append(parameter_object)

    return (instruction_object, success, error)
