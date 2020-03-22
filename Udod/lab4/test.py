import os

DIRECTORY = "Tests"
PRGNAME = "./lab4"

tests = os.listdir(DIRECTORY)

for i in range(len(tests)):

    testPath = DIRECTORY+"/"+tests[i]

    print("Test №", i, ": ", testPath, sep="")
    print(*open(testPath, "r").readlines(), sep="")
    os.system(PRGNAME+" "+testPath)
