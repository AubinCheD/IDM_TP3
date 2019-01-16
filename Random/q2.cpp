#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include "CLHEP/Random/MTwistEngine.h"



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
     for(int j=0;j<1000000000;++j){ 
		 randomNumber = mt->flat();
		 //std::cout << "Number :" << randomNumber << ";   ";
	 }
	 //std::cout << std::endl<<std::endl;
   }
   delete mt;
}


void restoreStatusFile(std::string fileName, int size){
	
	CLHEP::MTwistEngine * mt = new CLHEP::MTwistEngine();
	double randomNumber;
	std::stringstream ss;
    ss << "tirage_" << fileName;
	
	std::ofstream file;
	file.open (ss.str());
	
	//std::cout<<"Create : " << ss.str() << std::endl;
	
	mt->restoreStatus(fileName.c_str());
	for(int i=0;i<10;++i){
		randomNumber = mt->flat();	
	    file << "Number :" << randomNumber << std::endl;
	}
	file.close();
	delete mt;
}



int main ()
{
   int size = 10;

   createStatusFile(size);

   std::vector<std::thread *> threads(size);
   
   for(int i=0;i<size;++i){
	   std::stringstream ss;
       ss << "status" << i+1 << ".txt";
       std::string fileName = ss.str();
	   threads[i] = new std::thread(restoreStatusFile, fileName, size);
   }
   
   for(int i=0;i<size;++i){
	   threads[i]->join();
   }

   return 0;
}


