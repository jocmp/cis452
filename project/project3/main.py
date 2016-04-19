#!/usr/bin/env python
import getopt
import sys

from modules.DuController import DuController

main_view = None


def signal_handler(signal, frame):
    global main_view
    print "\n\nKeyboard interrupt.\n"
    sys.exit(0)

class MainView:

    def __init__(self):
        self.options, self.args = getopt.getopt(sys.argv[1:], "")
        self.controller = DuController()

    def run(self):
        if len(self.args) > 0:
            self.root_path = self.args[0]
        else:
            self.root_path = '.'
        self.whoa = self.controller.depthFirstSearch(self.root_path)
        print self.whoa

if __name__ == '__main__':
    main_view = MainView()
    main_view.run()