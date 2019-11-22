import re

def tokenize(file_name):
    with open(file_name, "r") as file:
        text = file.read()
    for c in ["[", "]", "{", "}", ";", ","]:
        text = text.replace(c, " %s " % c)
    text = re.sub(r"\s+", " ", text)
    tokens = [t for t in text.split(" ") if len(t) > 0]
    return tokens

def validate(token_list):
    pass

print(tokenize("test.asm"))