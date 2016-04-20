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
            options, args = getopt.getopt(sys.argv[1:], 'h', ['sort', 'count', 'csv'])
        except getopt.GetoptError as err:
            print str(err)
            sys.exit(2)
        self.flags = set()
        for o, a in options:
            if o == '-h':
                self.flags.add('h')
            elif o == '--sort':
                self.flags.add('s')
            elif o == '--count':
                self.flags.add('c')
            elif o == '--csv':
                self.flags.add('e')
            else:
                assert False, "unhandled option"
        self.controller = DuController()
        if len(args) > 0:
            self.root_path = self.controller.getAbsolutePath(args[0])
        else:
            self.root_path = '.'

    def run(self):
        self.all_paths = self.controller.depthFirstSearch(self.flags, self.root_path)
        if 's' in self.flags:
            self.all_paths.sort(key=lambda paths: paths[SIZE])
            for path in self.all_paths:
                self.controller.print_directory(self.flags, path)
        if 'e' in self.flags:
            self.controller.write_csv(self.all_paths)

if __name__ == '__main__':
    main_view = MainView()
    main_view.run()
