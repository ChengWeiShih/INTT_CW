#include "AnalyzeInttEvent.h"

R__LOAD_LIBRARY(libInttEvent.so)
R__LOAD_LIBRARY(libAnalyzeInttEvent.so)

void runAnalysis(const char * filename)
{
  //std::string save_dir = "/home/phnxrc/INTT/commissioning_5_23/hit_files/";
  std::string save_dir = "./";

  std::string out_file_name = filename;
  while(out_file_name.find("/") != std::string::npos)
  {
    out_file_name = out_file_name.substr(out_file_name.find("/") + 1);
  }
  out_file_name = out_file_name.substr(0, out_file_name.find("."));
  out_file_name += "_ana.root";
  out_file_name = save_dir + out_file_name;

  std::cout << out_file_name << std::endl;


  // read and generate InttTree

  if ( filename != NULL)
    {
      InitAnalysis(out_file_name.c_str());
      std::cout<<filename<<std::endl;
      RunAnalysis(filename);
    }

}
