from pathlib import Path
from shutil import copytree
from shutil import rmtree
from subprocess import check_output
from os import chdir

Import("env")

def flagExists(flag):
    buildFlags = env.ParseFlags(env["BUILD_FLAGS"])
    for define in buildFlags.get("CPPDEFINES"):
        if (define == flag or (isinstance(define, list) and define[0] == flag)):
            return True
            
def buildWeb():
    chdir("interface")
    print("Building interface with npm")
    try:
        print(check_output(["npm", "install"],shell=True).decode("utf-8"))
        print(check_output(["npm", "run", "build"],shell=True).decode("utf-8"))
        buildPath = Path("build")
        wwwPath = Path("../data/www")
        if wwwPath.exists() and wwwPath.is_dir():
            rmtree(wwwPath)        
        if not flagExists("PROGMEM_WWW"):
            print("Copying interface to data directory")
            copytree(buildPath, wwwPath)
    finally:
        chdir("..")

buildWeb()
