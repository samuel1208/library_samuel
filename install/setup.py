#!/home/fshen/Install_samuel/Python/python3.2.3/bin/python3.2
import version

import os
import subprocess
if "posix" == os.name:
    subprocess.call("/home/fshen/Install_samuel/Python/python3.2.3/lib", shell=True)

import buildUI
		
def main():
    a=buildUI.BuildUI(320,480)
    a.root.mainloop()
		
main()
