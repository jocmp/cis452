//
// Created by Josiah Campbell on 2/22/16.
//

#ifndef PROJECT2_NODE_H
#define PROJECT2_NODE_H

namespace gvsu {
    template<typename Z>
    class Node {
    public:
        Z data;
        Node *left;
        Node *right;
    };
}

#endif //PROJECT2_NODE_H

