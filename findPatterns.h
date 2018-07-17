#ifndef findPatterns_h
#define findPatterns_h

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>

using namespace std;

int find_place(string s, vector<string> v);
bool compare_patterns(string id1, string id2, map<string,vector<string> > strip_number, map<string,vector<string> > channel_number);
void process_data(vector<string> data);

#endif
