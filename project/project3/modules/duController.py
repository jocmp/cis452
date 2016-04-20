#!/usr/bin/env python
import os
import csv
from math import ceil
from datetime import date, datetime
import getpass as gp
import socket
import sys

TOP = 0

# Record values
SIZE = 0
PATH = 1
IS_DIR = 4
FILE_COUNT = 5

KILOBYTE = 1024
MEGABYTE = 1024 * KILOBYTE
GIGABYTE = 1024 * MEGABYTE


class DuController():
    discoveredPaths = set()

    def __init__(self):
        self.discoveredPaths = set()

    def getAbsolutePath(self, path):
        if not path.startswith('~'):
            return path
        else:
            return os.path.expanduser(path)

    def depthFirstSearch(self, flags, path):
        stat = os.stat(path)
        self.discoveredPaths.add(stat.st_ino)
        size = 0
        file_count = 0
        if os.path.isdir(path):
            sub_paths = []
            for item in os.listdir(path):
                item_absolute_path = os.path.join(path, item)
                if os.path.islink(item_absolute_path):
                    continue
                if os.stat(item_absolute_path).st_ino not in self.discoveredPaths:
                    sub_file = self.depthFirstSearch(flags, item_absolute_path)
                    size += sub_file[TOP][SIZE]
                    file_count += sub_file[TOP][FILE_COUNT]
                    sub_paths += sub_file
            size += stat.st_size
            access_time = self.get_date_format(stat.st_atime)
            modify_time = self.get_date_format(stat.st_mtime)
            directory = [(size, path, access_time, modify_time, True, file_count)] \
                        + [p for p in sub_paths if p[IS_DIR]]
            if not 's' in flags:
                self.print_directory(flags, directory[TOP])
            return directory
        elif os.path.isfile(path):
            """
            Return:
            * 0 size in Bytes
            * 1 Absolute Path
            * 2 Accessed
            * 3 Modified
            * 4 Is Directory
            * 5 File count (always 1)
            """
            access_time = self.get_date_format(stat.st_atime)
            modify_time = self.get_date_format(stat.st_mtime)
            if stat.size is None:
                print stat
                sys.exit(1)
            return [(stat.st_size, path, access_time, modify_time, False, 1)]

    def print_directory(self, flags, directory):
        output = ''
        if 'h' in flags:
            output += self.get_human_readable(directory[SIZE])
        else:
            output += str(directory[SIZE])
        if 'c' in flags:
            output += '\t' + str(directory[FILE_COUNT])
        output += '\t' + directory[PATH]
        print output

    def get_date_format(self, time):
        try:
            return datetime.fromtimestamp(int(time)).strftime("%Y-%m-%d %H:%M:%S")
        except ValueError:
            return ''

    def get_human_readable(self, size):
        if size < MEGABYTE:
            return str(ceil(size / KILOBYTE)) + 'K'
        elif size < GIGABYTE:
            return str(ceil(size / MEGABYTE)) + 'M'
        else:  # must be huuuuge
            return str(ceil(size / GIGABYTE)) + 'G'

    def write_csv(self, du_results):
        filename = 'proj3-' + datetime.now().strftime("%Y-%m-%d-%H:%M:%S") \
                   + '-' + socket.gethostname() + '-' + gp.getuser() + ".csv"
        with open(filename, 'wb') as csv_file:
            writer = csv.writer(csv_file, delimiter=',', quotechar='\'', quoting=csv.QUOTE_MINIMAL)
            for record in du_results:
                writer.writerow(record)
        csv_file.close()
