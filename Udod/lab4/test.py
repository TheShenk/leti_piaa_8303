import os

DIRECTORY = "Tests"
if os.name == "nt":
    PRGNAME = "lab4"
else:
    PRGNAME = "./lab4"

tests = os.listdir(DIRECTORY)

for i in range(len(tests)):

    testPath = DIRECTORY+"/"+tests[i]

    print("Test №", i, ": ", testPath, sep="")
    print(*open(testPath, "r").readlines(), sep="")
    os.system(PRGNAME+" "+testPath)
