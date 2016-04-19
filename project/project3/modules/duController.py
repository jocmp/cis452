#!/usr/bin/env python
import os
from math import ceil

SIZE = 0
PATH = 1
IS_DIR = 2
FILE_COUNT = 3
TOP = 0

KILOBYTE = 1000
MEGABYTE = 1000 * KILOBYTE
GIGABYTE = 1000 * MEGABYTE

class DuController():
    def __init__(self):
        self.discoveredPaths = set()

    def getAbsolutePath(self, path):
        if not path.startswith('~'):
            return path
        else:
            return os.path.expanduser(path)

    def depthFirstSearch(self, flags, path):
        absolutePath = self.getAbsolutePath(path)
        self.discoveredPaths.add(absolutePath)
        size = 0
        file_count = 0
        if os.path.isdir(absolutePath):
             sub_paths = []
             for item in os.listdir(absolutePath):
                 if item not in self.discoveredPaths:
                     sub_file = self.depthFirstSearch(flags, os.path.join(absolutePath, item))
                     size += sub_file[TOP][SIZE]
                     file_count += sub_file[TOP][FILE_COUNT]
                     sub_paths += sub_file
             size += os.stat(absolutePath).st_size
             directory = [(size, absolutePath, True, file_count)] + [p for p in sub_paths if p[IS_DIR]]
             if not 'sort' in flags:
                 self.print_directory(flags, directory[TOP])
             return directory
        elif os.path.isfile(absolutePath):
             return [(os.stat(absolutePath).st_size, absolutePath, False, 1)]


    def print_directory(self, flags, directory):
        output = ''
        if 'human readable' in flags:
            output += self.get_human_readable(directory[SIZE])
        else:
            output += str(directory[SIZE])
        if 'count' in flags:
            output += '\t' + str(directory[FILE_COUNT])
        output += '\t' + directory[PATH]
        print output


    def get_human_readable(self, size):
        if size < MEGABYTE:
            return str(ceil(size / KILOBYTE)) + 'K'
        elif size < GIGABYTE:
            return str(ceil(size / MEGABYTE)) + 'M'
        else: # must be huuuuge
            return str(ceil(size / GIGABYTE)) + 'G'