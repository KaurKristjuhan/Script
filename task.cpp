#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>

using namespace std;

vector<string> errors;

int find_place(string s,vector <string> v){
  for(int i=0;i<v.size();i++){
    if(s==v[i]) return i;
  }
  return -1;
}

bool compare_patterns(string id1, string id2, map<string,vector<string> > strip_number, map<string,vector<string> > channel_number){
  bool result = true;
  if (strip_number[id1].size()!=128) result=false;
  if (strip_number[id1].size()!=128) result=false;
  for(int i=0;i<strip_number[id1].size();i++){
     int place = find_place(strip_number[id1][i],strip_number[id2]);
     if(place==-1){
       errors.push_back(strip_number[id1][i]+" missing in "+id2);
       result=false;
      }
     else if(channel_number[id1][i]!=channel_number[id2][place]) result=false;
  }
  return result;
}

void process_data(vector<string> data){
  ofstream foutp("patterns.txt");
  ofstream foute("errors.txt");
  //structured data stored in here (9 elements per line)
  vector<vector<string> > v;
  for(int i=0;i<9;i++){
    vector<string> q;
    v.push_back(q);
  }
  int line_number=1;
  for(int i=0;i<data.size();i++){
    stringstream ss(data[i]);
    string buf;
    int counter=0;
    while(ss>>buf){
      v[counter%9].push_back(buf);
      counter++;
    }
    if(counter!=9){
      cout<<endl<<"Error on line "<<line_number<<":"<<endl<<data[i]<<endl<<"Only processing data before this line."<<endl;
      for(int i=0;i<counter-1;i++){
	v[counter%9].pop_back();
      }
      break;
    }
    line_number++;
  }
  map<string,set<string> > ms; //a map from ID-s to the info about the position of the vfat
  map<string,vector<string> > strip_number;
  map<string,vector<string> > channel_number;
  vector <string> ids;
  for(int i=0;i<v[7].size();i++){
    if(ms.find(v[7][i])==ms.end()){
      set<string> st;
      vector<string> vs;
      vector<string> vc;
      ms[v[7][i]]=st;
      strip_number[v[7][i]]=vs;
      channel_number[v[7][i]]=vc;
      ids.push_back(v[7][i]); //vfatId
    }
    ms[v[7][i]].insert(v[0][i]); //vfat_position
    ms[v[7][i]].insert(v[1][i]); //z_direction
    ms[v[7][i]].insert(v[2][i]); //iEta
    ms[v[7][i]].insert(v[3][i]); //iPhi
    ms[v[7][i]].insert(v[4][i]); //depth
    strip_number[v[7][i]].push_back(v[5][i]); //strip_number
    channel_number[v[7][i]].push_back(v[6][i]); //vfat_chnnel_number
    ms[v[7][i]].insert(v[8][i]); //sec
  }
  vector<bool> checked; //True if a vfat has been assigned (or denied) a pattern
  vector<vector<string> > patterns;
  for(int i=0;i<ids.size();i++){
    checked.push_back(false);
  }
  for(int i=0;i<ids.size();i++){
    if(checked[i]==true) continue;
    vector<string> p; //contains all the ID-s of the current pattern
    for(int j=i;j<ids.size();j++){
      if(checked[j]==true) continue;
      if(compare_patterns(ids[i],ids[j],strip_number,channel_number)==true){
	checked[j]=true;
	p.push_back(ids[j]);
	continue;
      }
      if(strip_number[ids[j]].size()!=128) checked[j]=true;
    }
    if(p.size()!=0){
      patterns.push_back(p);
      foutp<<"##############################################"<<endl<<"Pattern "<<patterns.size()<<endl<<endl;
      foutp<<"Mapping:"<<endl;
      for(int j=0;j<128;j++){
	foutp<<strip_number[p[0]][j]<<" <->  "<<channel_number[p[0]][j]<<endl;
      }
      foutp<<endl<<"ID's:"<<endl;
      for(int j=0;j<p.size();j++){
	foutp<<p[j]<<endl;
      }
    }
  }
  compare_patterns(ids[1],ids[0],strip_number,channel_number);
  errors.push_back("");
  for(int i=0;i<ids.size();i++){
    if(strip_number[ids[i]].size()!=128){
      int a = strip_number[ids[i]].size();
      stringstream ss;
      ss << a;
      string str = ss.str();
      errors.push_back(str+" strips for "+ids[i]);
    }
  }
  errors.push_back("");
  for(int i=0;i<ids.size();i++){
    if(ms[ids[i]].size()!=6){
      errors.push_back("Mismatching info for "+ids[i]+":");
      set<string>::iterator it = ms[ids[i]].begin();
      while (it != ms[ids[i]].end()){
	errors.push_back(*it);  
	it++;
      }
      errors.push_back("");
    }
  }
  cout<<endl<<"Number of ID-s: "<<ids.size()<<endl;
  cout<<"Patterns written in patterns.txt"<<endl;
  cout<<"Errors written in file errors.txt"<<endl;
  for(int i=0;i<errors.size();i++){
    foute<<errors[i]<<endl;
  }
}

int main(int argc, char *argv[]){
  for(int i=1;i<argc;i++){
    cout<<argv[i]<<endl;
  }
  ifstream fin;
  fin.open("test (copy).out");
  string s;
  getline(fin,s); //"not slice test"
  vector<string> data;
  while(getline(fin,s)){
    data.push_back(s);
  }
  process_data(data);
  return 0;
}
