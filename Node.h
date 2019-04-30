#ifndef NODE_H
#define NODE_H

using namespace std;

#include <vector>
#include <string>

struct Node{
        Node * parent;
        int element_offset;
        int element_length;
        bool isDir;
        string element_name;
        vector<Node *> child;
        char * lump;
        string path;
};

Node *newNode(int offset, int length, string name, bool dir, Node * n, char * lump, string path);

Node *headNode(int offset, int length, string name, bool dir);

#endif // NODE_H
