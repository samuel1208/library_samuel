import sys
import os

def IsPython3Check() :
    print("OS Version: %s"%(os.name))
    print("Python version: %s"%(sys.version[0:6]))
    IsPython3 = sys.version[0] 
    return IsPython3

###default is tuple
IsPython3 = IsPython3Check()
