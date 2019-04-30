#include <Wad.h>
#include<node.h>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

#include <string>
#include <vector>




//default constructor
Wad::Wad(){
	wmagic;
    vector<Node*> wnodes;
}

 //Constructor
Wad::Wad(string magic, vector<Node*> nodes){
	wmagic = magic;
    wnodes = nodes;
    //check if created properly
    //cout << " vector size: " << wnodes.size() << " magic: " << wmagic;
}

Wad* Wad::loadWad(const string &path){
    Wad * w;
	ifstream f(path.c_str());
	string w_magic;

	//wad ifstream to char array
	if(f){
        f.seekg(0, f.end);
        //length of charArray
        int l = f.tellg();
        f.seekg(0,f.beg);
        char * buf = new char [l];
	f.read(buf,l);


    //magArray is the correct Magic to send to constructor
    char  magArray[5];
    for(int i = 0 ; i <= 3; i++ ){
        magArray[i] = buf[i];
        if(magArray[i] != 0){
            w_magic += magArray[i];
        }
    }

    int descr_count = int((unsigned char)(buf[7]) << 24 |
            (unsigned char)(buf[6]) << 16 |
            (unsigned char)(buf[5]) << 8 |
            (unsigned char)(buf[4]));

    int descr_offset = int((unsigned char)(buf[11]) << 24 |
            (unsigned char)(buf[10]) << 16 |
            (unsigned char)(buf[9]) << 8 |
            (unsigned char)(buf[8]));


    int data_tracker = descr_offset;
    int element_offset;
    int element_length;
    char elementName[9];
    string element_name;

    int newOffset = data_tracker;
    //prints header data
    /*cout << "\n" << magArray << "\n";
    cout << "\n" << descr_count << "\n";
    cout << "\n" << descr_offset << "\n";*/

    string root = "r";
    Node * rootNode = headNode(0, 0, root, true);
    vector<Node*> nodeVector;
    nodeVector.push_back(rootNode);

    //This for loop gives the element offset, length, and name for every descriptor.
    Node * currentParent = rootNode;
    Node * currentNode;
    int contentCount=0;
    bool mapDirContent = false;
    bool nameDirContent = false;
    bool isEnd = false;


    string path;
    for(int i = 0; i<descr_count; i++){
        newOffset = data_tracker;

        element_offset = int((unsigned char)(buf[newOffset + 3]) << 24 |
            (unsigned char)(buf[newOffset + 2]) << 16 |
            (unsigned char)(buf[newOffset + 1]) << 8 |
            (unsigned char)(buf[newOffset + 0]));

        element_length = int((unsigned char)(buf[newOffset + 7]) << 24 |
            (unsigned char)(buf[newOffset + 6]) << 16 |
            (unsigned char)(buf[newOffset + 5]) << 8 |
            (unsigned char)(buf[newOffset + 4]));

        int k=8;

        for(int j = 0 ; j < 8; j++ ){
            elementName[j] = buf[newOffset + k];
            k++;
            if(elementName[j]!= 0){
                element_name += elementName[j];
            }
        }

        //determines if we have  a a directory, and if so, what kind.
        bool isDir = false;
        bool mapDir = false;
        bool nameDir = false;
        if(element_length == 0){
            isDir = true;
        }
       if (isDir && elementName[0]=='E' && elementName[2]=='M'){
            mapDir = true;
       }
       else if ((isDir && elementName[2]=='_' && elementName[3]=='S' && elementName[4]=='T' && elementName[5]=='A' && elementName[6]=='R' && elementName[7]=='T' )|| (isDir && elementName[1]=='_' && elementName[2]=='S' && elementName[3]=='T' && elementName[4]=='A' && elementName[5]=='R' && elementName[6]=='T')){
            nameDir = true;
       }
        else if ((isDir && elementName[2]=='_' && elementName[3]=='E' && elementName[4]=='N' && elementName[5]=='D')|| (isDir && elementName[1]=='_' && elementName[2]=='E' && elementName[3]=='N' && elementName[4]=='D')){
            isEnd = true;
       }
       else{
            isDir = false;
       }

       if (isDir && nameDir && elementName[2] == '_'){
            element_name = elementName[0];
            element_name += elementName[1];
       }
       else if(isDir && nameDir && elementName[1] == '_'){
           element_name = elementName[0];
       }

       //finds lump data;
        char lump[element_length];
        if (!isDir){
            for(int h = 0; h<element_length; h++){
               lump[h] = buf[element_offset+h];
            }
        }

        //creates path
        if(currentParent->element_name == "r"){
            path += "/";
            path += element_name;
        }
        if(currentParent->element_name != "r" && !isEnd){
            path = currentParent->path;
            path += "/";
            path += element_name;
        }
        if(isEnd){
            path = "";
        }

       //creates node
        Node * myNode = newNode(element_offset, element_length, element_name, isDir, currentParent, lump, path);
        currentNode = myNode;
        element_name = "";

        //adjusts path
        if(!isDir && !isEnd){
            string subst = "/";
            subst+=element_name;
            int substrLength = subst.size()-1;
            int pathLength = path.size();
            int erasePoint = pathLength-substrLength;
            path.erase(0,erasePoint);
        }

        //pushes to vector
        if(!isEnd){
            nodeVector.push_back(myNode);
        }

        //WE HAVE CHANGED TO A MAP DIRECTORY PARENT
        if(isDir && mapDir && contentCount <10){
            currentParent = currentNode;
            mapDirContent = true;
        }

        if(contentCount < 11 && mapDirContent == true){
            if (contentCount == 10 && !isDir){
                currentParent = currentParent->parent;
                mapDirContent = false;
            }
            contentCount = contentCount+1;
            if(contentCount ==11){
                contentCount = 0;
            }
        }


        //WE HAVE CHANGED TO A NAMESPACE DIRECTORY PARENT
        if(isDir && nameDir){
            currentParent = currentNode;
            nameDirContent = true;
        }
        if(isEnd){
            currentParent = currentParent->parent;
            isEnd = false;
        }
        if(nameDirContent && !isEnd){
            nameDirContent = false;
        }

        //increments buffer
        data_tracker = data_tracker+16;

    }

    w = new Wad(w_magic, nodeVector);

	f.close();
	delete[] buf;
	}
	else{
        cout << "Bad path";
	}
    return w;
}

char* Wad::getMagic(){
    char buf[4];
    strcpy(buf, wmagic.c_str());
	return buf;
}

bool Wad::isContent(const string &path){
	if(!isDirectory(path)){
		return true;
	}
	else{
		return false;
	}
}

bool Wad::isDirectory(const string &path){
        int c = wnodes.size();
        string adjusted;
        for(int i = 1; i<c; i++) {
            adjusted = wnodes[i]->parent->path;
            adjusted += "/";
            if(wnodes[i]->isDir && wnodes[i]->parent->path == path || adjusted == path){
                return true;
            }
            else if(!wnodes[i]->isDir && wnodes[i]-> path == path){
                return false;
            }
            else{
            }
        }
}

int Wad::getSize(const string &path){
    if(isContent(path)){
        string adjusted;
        int c = wnodes.size();
        int number;
        for(int i = 1; i<c; i++) {
            adjusted = wnodes[i]->path;
            adjusted += "/";
            if(wnodes[i]->path == path || adjusted == path){
                number = wnodes[i]->element_length;
            }
        }
        return number;
    }
    else{
        return -1;
    }
}

int Wad::getContents(const string &path, char *buffer, int length, int offset /*= 0*/){
	if (!isContent(path)){
		return -1;
	}
	else{
        int c = wnodes.size();
        string adjusted;
        int numberOfBytes = 0;
        for(int i = 1; i<c; i++) {
            adjusted = wnodes[i]->path;
            adjusted += "/";
            if (wnodes[i]->path == path || adjusted == path){
                int j = 0;
                for (int k = offset; j<length; j++){
                    buffer[j] = wnodes[i]->lump[k];
                    k++;
                    numberOfBytes++;
                    cout << buffer[j];
                }
            }
        }
        return numberOfBytes;
	}
}

int Wad::getDirectory(const string &path, vector<string> *directory){
    if (!isDirectory(path)){
		return -1;
	}
	else{
        int elementsAtLevel = 0;
        int c = wnodes.size();
        string adjusted;
        for(int i = 1; i<c; i++) {
            adjusted = wnodes[i]->parent->path;
            adjusted += "/";
            if(wnodes[i]->parent->path == path || adjusted == path){
                directory->push_back(wnodes[i]->element_name);
                elementsAtLevel = elementsAtLevel+1;

            }
            //adjusted = "";
        }
        return elementsAtLevel;
	}
}

