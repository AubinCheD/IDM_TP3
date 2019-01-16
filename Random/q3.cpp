#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "CLHEP/Random/MTwistEngine.h"

double piFromStatusFile (std::string fileName, int nbDraws);

int main (int argc, char ** argv)
{
   if(argc == 2){
	   std::cout << "PI = " << piFromStatusFile(argv[1],1000000) << std::endl;

   }else{
	   std::cout << "Veuillez passer en parametre le nom du fichier de status" << std::endl;
   }

   return 0;
}


double piFromStatusFile (std::string fileName, int nbDraws)
{	
	int count=0;
	int size=nbDraws;
    double x,y, res;
    CLHEP::MTwistEngine * mt = new CLHEP::MTwistEngine();
   
    mt->restoreStatus(fileName.c_str());
    for(int i=0;i<size;++i){
	   x = mt->flat();
	   y = mt->flat();	
	   if(x*x+y*y < 1){
		   count++;
	   }
    }
   
    delete mt;
    
    return ((double)count)/size*4;
}

