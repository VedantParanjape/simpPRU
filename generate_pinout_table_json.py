import json

pinout = {}

for i in range(2):
    pinout["pru"+str(i)] = {}
    temp = {}

    while True:
        pru_pin = int(input(f"[pru{i}] pru pin number: "))
        
        if (pru_pin < 0):
            break

        header_pin  = input("\theader pin: ")
        mode = int(input("\tmode (1 for in, 2 for out, 3 for in+out): "))
        
        temp[pru_pin] = [header_pin, mode]
        
    pinout["pru"+str(i)] = temp

with open("src/pinout.json", "w+") as fhandle:
    json.dump(pinout, fhandle)
