path = input("Enter file path: ")

with open(path, "r") as f:
    stringify = ""
    while 1:
        char = f.read(1)

        if not char:
            break

        if char == '\t':
            stringify = stringify + '\\t'
        elif char == '\n':
            stringify = stringify + '\\n'
        else:
            stringify = stringify + char

    with open("out.txt", "w+") as out:
        out.write(stringify)