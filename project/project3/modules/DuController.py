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
             for item in os.listdir(absolutePath):
                 if item not in self.discoveredPaths:
                     size += self.depthFirstSearch(os.path.join(absolutePath, item))
             print str(size) + '\t' + absolutePath
             return size
        elif os.path.isfile(absolutePath):
             print str(os.stat(absolutePath).st_size) + '\t' + absolutePath
             return os.stat(absolutePath).st_size
