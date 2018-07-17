#include "genLines.h"
#include "findPatterns.h"

using namespace std;

int main(int argc, char **argv){
  int save=0;
  int list=1;
  int maxVfat=30;
  if (argc>1) {
    for (int ia=0; ia<argc; ia++) {
      if (std::string(argv[ia])=="-save") {
	std::cout << " * save=1\n";
	save=1;
      }
      else if (std::string(argv[ia])=="-no-list") {
	std::cout << " * list=0\n";
	list=0;
      }
      else if (std::string(argv[ia])=="-max-vfat") {
	if (ia+1==argc) {
	  std::cout << " -max-vfat needs a value\n";
	  return 1;
	}
	maxVfat=atoi(argv[ia+1]);
	ia++;
	std::cout << " * maxVfat=" << maxVfat << "\n";
      }
      else if (std::string(argv[ia])=="-help") {
	std::cout << "Possible arguments:\n"
		  << "sampleGen [-save] [-no-list] [-max-vfat value]\n";
	return 0;
      }
    }
  }
  vfat_def::init();
  std::vector<std::string> linesRnd;
  generateLinesRnd(maxVfat,30,linesRnd);
  std::cout << "got " << linesRnd.size() << " lines\n";
  process_data(linesRnd);

// optional features
  if (save) {
    std::ofstream fout("test-rnd.out");
    for (unsigned int i=0; i<linesRnd.size(); i++) {
      fout << linesRnd[i] << "\n";
    }
    fout.close();
    std::cout << "File test-rnd.out created\n";
  }

  if (list) {
    for (unsigned int i=0; i<linesRnd.size(); i++) {
      std::cout << linesRnd[i] << "\n";
    }
  }
  return 0;
}
