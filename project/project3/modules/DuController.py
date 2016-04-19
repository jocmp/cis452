#!/usr/bin/env python
import os

class DuController():
    def __init__(self):
        self.discoveredPaths = set()

    def getAbsolutePath(self, path):
        if not path.startswith('~'):
            return path
        else:
            return os.path.expanduser(path)

    def depthFirstSearch(self, path):
        absolutePath = self.getAbsolutePath(path)
        self.discoveredPaths.add(absolutePath)
        size = 0
        if os.path.isdir(absolutePath):
             sub_files = []
             for item in os.listdir(absolutePath):
                 if item not in self.discoveredPaths:
                     sub_file = self.depthFirstSearch(os.path.join(absolutePath, item))
                     size += sub_file[0][0]
                     sub_files.append(sub_file)
             size += os.stat(absolutePath).st_size
             print str(size) + '\t' + absolutePath
             return [(size, absolutePath)] + sub_files
        elif os.path.isfile(absolutePath):
             return [(os.stat(absolutePath).st_size, absolutePath)]
