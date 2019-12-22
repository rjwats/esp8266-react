from shutil import copytree
from shutil import rmtree
from subprocess import check_output
import os import chdir

Import("env")

def build_web():
    chdir("interface")
    print("Building www...")
    try:
        print(check_output(["npm", "install"]))
        print(check_output(["npm", "run", "build"]))
        rmtree("../data/www")
        copytree("build", "../data/www")
    finally:
        chdir("..")

build_web()
