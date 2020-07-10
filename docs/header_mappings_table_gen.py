import json

with open("../src/pocket_beagle_pinout.json", "r") as fp:
    js = json.load(fp)

fout = open("header_mapping.md", "w")

for k in range(2):
    pru = js["pru"+str(k)]

    for i in pru.keys():
        for j in pru[i]:
            if j[1] == 1:
                fout.writelines(f"| pru{str(k)} | - | {i} | {j[0]} |\n")
            elif j[1] == 2:
                fout.writelines(f"| pru{str(k)} | {i} | - | {j[0]} |\n")
            elif j[1] == 3:
                fout.writelines(f"| pru{str(k)} | {i} | {i} | {j[0]} |\n")

fout.close()