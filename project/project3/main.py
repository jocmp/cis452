#!/usr/bin/env python
import getopt
import sys

from modules.duController import DuController


SIZE = 0

def signal_handler(signal, frame):
    global main_view
    print "\n\nKeyboard interrupt.\n"
    sys.exit(0)

class MainView:

    def __init__(self):
        try:
            options, args = getopt.getopt(sys.argv[1:], 'h', ['sort', 'count'])
        except getopt.GetoptError as err:
            print str(err)
            sys.exit(2)
        self.flags = set()
        for o, a in options:
            if o == '-h':
                self.flags.add('human readable')
            elif o == '--sort':
                self.flags.add('sort')
            elif o == '--count':
                self.flags.add('count')
            else:
                assert False, "unhandled option"
        if len(args) > 0:
            self.root_path = args[0]
        else:
            self.root_path = '.'
        self.controller = DuController()

    def run(self):
        if 'sort' in self.flags:
            self.all_paths = self.controller.depthFirstSearch(self.flags, self.root_path)
            self.all_paths.sort(key=lambda paths: paths[SIZE])
            for path in self.all_paths:
                self.controller.print_directory(self.flags, path)
        else:
            self.controller.depthFirstSearch(self.flags, self.root_path)

if __name__ == '__main__':
    main_view = MainView()
    main_view.run()