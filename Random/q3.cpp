#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "CLHEP/Random/MTwistEngine.h"

double piFromStatusFile (std::string fileName, int nbDraws);
double averagePi(int nbDraws);
void createStatusFile(int size);

int main (int argc, char ** argv)
{
   if(argc == 2){
	   std::cout << "PI = " << piFromStatusFile(argv[1],1000000) << std::endl;
	   std::cout << "Average PI = " << averagePi(10) << std::endl;
	   

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

double averagePi(int nbDraws)
{
	createStatusFile(nbDraws);
	
	double pi = 0;
    for (int i=0; i<10; ++i)
    {
		std::stringstream ss;
		ss << "status" << i+1 << ".txt";
		pi+=piFromStatusFile(ss.str(),1000000);
	}
	return pi/10;
}

void createStatusFile(int size){
	
   CLHEP::MTwistEngine * mt = new CLHEP::MTwistEngine();
   double randomNumber;

   for(int i = 1; i <= size; i++)
   {
     std::stringstream ss;
     ss << "status" << i << ".txt";
     const char *s = (ss.str()).c_str();
     mt->saveStatus(s);
     //std::cout << s << " : " << std::endl;
     for(int j=0;j<50000;++j){ 
		 randomNumber = mt->flat();
		 //std::cout << "Number :" << randomNumber << ";   ";
	 }
	 //std::cout << std::endl<<std::endl;
   }
   delete mt;
}
