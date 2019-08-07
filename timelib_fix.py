import os
import sys
import re
Import("env")

# Find files under 'root' of a given 'fileName' in directories matching 'subDirectoryPattern'
# This will allow us to safely find the offending Time.h file for removal prior to building
def findSubDirectoryFiles(root, subDirectoryPattern, fileName):
    subDirectories = os.listdir(root)
    subDirectories = filter(lambda d: re.match(subDirectoryPattern, d), subDirectories)  
    result = []
    for subDirectory in subDirectories:
        candidateFile = os.path.join(root, subDirectory, fileName)
        if os.path.isfile(candidateFile):
            result.append(candidateFile)
    return result

def deleteTimeHeader(libDepsDir):
    timeHeaderFile = "Time.h"
    timeLibDirectoryPattern = "Time(_ID[0-9]+)?"

    # delete the file, as long as we only find one
    if os.path.isdir(libDepsDir) :
        deletionCandidates = findSubDirectoryFiles(libDepsDir, timeLibDirectoryPattern, timeHeaderFile)
        numDeletionCandidates = len(deletionCandidates)
        if numDeletionCandidates == 1:
            os.remove(deletionCandidates[0])
        elif numDeletionCandidates > 1:        
            os.write(2, 'Can\'t delete Time.h, more than one instance found:\n' + '\n'.join(deletionCandidates))
            sys.exit(1)

# old lib deps directory
deleteTimeHeader(os.path.join(env.subst('$PROJECT_DIR'), '.piolibdeps'))

# new lib deps directory
deleteTimeHeader(os.path.join(env.subst('$PROJECTLIBDEPS_DIR'), env.subst('$PIOENV')))
