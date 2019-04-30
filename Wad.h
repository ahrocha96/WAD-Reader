#ifndef WAD_H
#define WAD_H

#include <fstream>
#include <iostream>
using namespace std;

#include <Node.h>
#include <string>
#include <vector>

 class Wad
 {


	public:
	    string wmagic;

        vector<Node*> wnodes;

		Wad();

		Wad(string magic, vector<Node*> nodes);

		static Wad* loadWad(const string &path);

		char* getMagic();

		bool isContent(const string &path);

		bool isDirectory(const string &path);

		int getSize(const string &path);

		int getContents(const string &path, char *buffer, int length, int offset = 0);

		int getDirectory(const string &path, vector<string> *directory);

 };








 #endif
