from pathlib import Path
from shutil import copytree
from shutil import rmtree
from subprocess import check_output, Popen, PIPE, STDOUT, CalledProcessError
from os import chdir

Import("env")

def flagExists(flag):
    buildFlags = env.ParseFlags(env["BUILD_FLAGS"])
    for define in buildFlags.get("CPPDEFINES"):
        if (define == flag or (isinstance(define, list) and define[0] == flag)):
            return True

def executeWithPopen(command):
    with Popen(command, shell=True, stdout=PIPE, universal_newlines=True) as p:
        for line in p.stdout:
            print(line, end='')
        p.wait()
        if p.returncode != 0:                
            raise CalledProcessError(p.returncode, p.args)

def buildWeb():
    chdir("interface")
    print("Building interface with npm")
    try:
        executeWithPopen("npm install")
        executeWithPopen("npm run build")
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
