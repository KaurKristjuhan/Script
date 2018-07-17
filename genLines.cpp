#include "genLines.h"

std::vector<int> vfat_def::activeSectors;
std::map<int,std::vector<std::string> > vfat_def::patterns;

// -----------------------------------------------------------------

namespace vfat_def {
void init()
{
  activeSectors.push_back(1);
  activeSectors.push_back(2);
  activeSectors.push_back(3);
  activeSectors.push_back(4);
  activeSectors.push_back(5);

  if (!loadPattern(0,"dir-data/pattern_0.dat",patterns) ||
      !loadPattern(1,"dir-data/pattern_1.dat",patterns) ||
      !loadPattern(2,"dir-data/pattern_2.dat",patterns) ||
      !loadPattern(3,"dir-data/pattern_3.dat",patterns)) {
    std::cout << "Error: failed to load patterns\n";
  }
}
};

// -----------------------------------------------------------------

int loadPattern(int index, const char *fname,
		std::map<int,std::vector<std::string> > &pattern_map)
{
  std::ifstream fin(fname);
  if (!fin.is_open()) {
    std::cout << "loadPattern: failed to open the file\n";
    return 0;
  }

  std::vector<std::string> mpv;
  std::string s;
  while (!fin.eof() && getline(fin,s)) {
    mpv.push_back(s);
  }
  fin.close();

  pattern_map[index]= mpv;

  return 1;
}

// -----------------------------------------------------------------

int checkInit()
{
  if ((vfat_def::activeSectors.size()==0) ||
      (vfat_def::patterns.size()==0)) {
    std::cout << "Run-time Error: the code should call vfat_def::init() before calling functions from genLines module\n";
    return 0;
  }
  return 1;
}

// -----------------------------------------------------------------

int generateLines(int nVfats, unsigned int nSects, std::vector<std::string> &lines)
{
  if (!checkInit()) return 0;

  int countVfats=0;

  std::vector<int> useSectors=vfat_def::activeSectors;
  if (useSectors.size()<nSects) {
    useSectors.clear();
    for (unsigned int i=0; i<nSects; i++) {
      useSectors.push_back(i+1);
    }
  }

  for (unsigned int isec=0; isec<nSects; isec++) {
    if (isec>useSectors.size()) {
      std::cout << "too many sectors\n";
      return 0;
    }

    int sector=useSectors[isec];

    for (int iDepth=1; iDepth<=2; iDepth++) {
      for (int iPhi= sector*3-2; iPhi<= sector*3; iPhi++) {
	for (int iEta=vfat_def::_nIEta_min; iEta<=vfat_def::_nIEta_max; iEta++) {
	  int vfat=iDepth*10000+iEta*1000+iPhi;

	  int iPattern=iRand(0,3);

	  std::stringstream oss1;
	  oss1 << "vfat_position=" << (vfat_def::_nIEta_max-iEta)
	       << " z_direction=-1"
	     << " iEta=" << iEta << " iPhi=" << iPhi
	     << " depth=" << iDepth;

	  countVfats++;
	  if (countVfats>nVfats) return 1;

	  std::vector<std::string> *sv= & vfat_def::patterns.at(iPattern);
	  int strip,vfatChan;
	  for (unsigned i=0; i<sv->size(); i++) {
	    std::stringstream iss(sv->at(i));
	    iss >> strip >> vfatChan;
	    std::stringstream oss;
	    oss << oss1.str() << " strip_number=" << strip
		<< " vfat_chnnel_number=" << vfatChan
		<< " vfatId=0x" << std::hex << vfat << std::dec
		<< " sec=" << (100+sector);
	    if (0 && (strip==0)) {
	      std::cout << oss.str() << "\n";
	      std::cout << "  iPattern=" << iPattern << "\n";
	    }
	    lines.push_back(oss.str());
	  }
	}
      }
    }
  }

  return 1;
}


// -----------------------------------------------------------------

int generateLinesRnd(int nVfats, unsigned int nSects,
		     std::vector<std::string> &lines)
{
  std::vector<std::string> tmpLines;
  if (!generateLines(nVfats,nSects,tmpLines)) return 0;
  std::cout << " generated " << tmpLines.size() << " lines\n";

  unsigned int imax=tmpLines.size();
  for (unsigned int i=0; i<imax; i++) {
    int ipos= iRand(1,tmpLines.size());
    lines.push_back(tmpLines.at(ipos-1));
    tmpLines.erase(tmpLines.begin()+ipos-1);
    if ((imax>20000) && (((i+1)*10/imax)-i*10/imax > 0)) {
      std::cout << " randomization: " << tmpLines.size() << " remaining\n";
    }
  }
  return 1;
}

// -----------------------------------------------------------------

