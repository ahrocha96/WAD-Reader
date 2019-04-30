#include "Node.h"
#include <fstream>
#include <iostream>
using namespace std;
#include <string>


Node *newNode(int offset, int length, string name, bool dir, Node * n, char * data, string p){
        Node *temp = new Node;
        temp->element_offset = offset;
        temp->element_length = length;
        temp->element_name = name;
        temp->isDir = dir;
        temp->parent = n;
        temp->lump = data;
        temp->path = p;
        //this line checks node data
        //cout << " Node: "  << " " << temp->element_offset << " " << temp->element_length << " " <<  temp->element_name << " "
        //<<  temp->isDir <</* " lump: "<< temp->lump <<*/" Current parent:  " <<  n->element_name<< " Path: " << temp->path <<"  SUCCESS\n\n\n\n\n";
        return temp;
}


Node *headNode(int offset, int length, string name, bool dir){
        Node *temp = new Node;
        temp->element_offset = offset;
        temp->element_length = length;
        temp->element_name = name;
        temp->isDir = dir;
        temp->parent = NULL;
        temp->path = "/";
        //cout << " Node: " << temp->element_offset << temp->element_length << temp->element_name << temp->isDir << "SUCCESS\n\n\n\n\n";
       // temp->element_data = data;
        return temp;
}
