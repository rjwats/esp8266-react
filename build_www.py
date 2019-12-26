from shutil import copytree
from shutil import rmtree
from subprocess import check_output
import os import chdir

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
        print(check_output(["npm", "install"]))
        print(check_output(["npm", "run", "build"]))
        if not defined("PROGMEM_WWW"):
            rmtree("../data/www")
            copytree("build", "../data/www")
    finally:
        chdir("..")

build_web()
