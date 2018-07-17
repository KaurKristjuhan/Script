#ifndef getLines_h
#define getLines_h

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>

namespace vfat_def {
  typedef enum { _nIEta_min=1, _nIEta_max=16 } TIEtaPos_t;
  extern std::vector<int> activeSectors;
  extern std::map<int, std::vector<std::string> > patterns;

  void init();
}

inline int iRand(int iMin, int iMax)
{
  int i= rand() % (iMax-iMin + 1) + iMin;
  return i;
}

int checkInit();
int loadPattern(int index, const char *fname,
		std::map<int,std::vector<std::string> > &pattern_map);

int generateLines(int nVfats, unsigned int nSects, std::vector<std::string> &lines);
int generateLinesRnd(int nVfats, unsigned int nSects, std::vector<std::string> &lines);

#endif
