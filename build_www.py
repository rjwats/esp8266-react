from pathlib import Path
from shutil import copytree
from shutil import rmtree
from subprocess import check_output
from os import chdir

Import("env")

def defined(flag):
    my_flags = env.ParseFlags(env["BUILD_FLAGS"])
    for define in my_flags.get("CPPDEFINES"):
        if (define == flag or (isinstance(define, list) and define[0] == flag)):
            return True
            
def build_web():
    chdir("interface")
    print("Building www...")
    try:
        print(check_output(["npm", "install"],shell=True).decode("utf-8"))
        print(check_output(["npm", "run", "build"],shell=True).decode("utf-8"))
        if not defined("PROGMEM_WWW"):
            buildPath = Path("build")
            wwwPath = Path("../data/www")
            if wwwPath.exists() and wwwPath.is_dir():
                rmtree(wwwPath)
            copytree(buildPath, wwwPath)
    finally:
        chdir("..")

build_web()
