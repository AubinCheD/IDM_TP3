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
#include <cmath>

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
     for(int j=0;j<50000;++j){ 
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

int trySentence (std::string fileName, std::string sentence)
{
	CLHEP::MTwistEngine * mt = new CLHEP::MTwistEngine();
	double randomNumber;
	bool found = false;
	int count=0;
	
	mt->restoreStatus(fileName.c_str());
	
	while(!found)
	{
		std::string word = "";
		for (int i=0; i<sentence.size(); ++i) 
		{
			randomNumber = mt->flat();
			std::stringstream a;
			a << (char)(randomNumber*128 + 1);
			word += a.str();
		}
		++count;
		std::cout<<word<<std::endl;
		if(word == sentence)
			found=true;	
	}
	
	delete mt;
	return count;
}

int drawGattaca(std::string fileName){
	CLHEP::MTwistEngine * mt = new CLHEP::MTwistEngine();
	double randomNumber;
	bool found = false;
	int count=0;
	
	mt->restoreStatus(fileName.c_str());
	
	while(!found)
	{
		std::string word = "";
		for (int i=0; i<7; ++i) 
		{
			randomNumber = mt->flat();
			
			if(randomNumber<=0.25)
				word +='a';
			else if(randomNumber<=0.50)
				word +='c';
			else if(randomNumber<=0.75)
				word +='g';
			else
				word +='t';
		}
		++count;
		if(word == "gattaca")
			found=true;	
	}
	
	delete mt;
	return count;
}


void threadGattaca(std::string fileName, int * value){
	*value = drawGattaca(fileName.c_str());
}

void averageGattaca(int nbSimulations)
{
	std::vector<std::thread> threads(nbSimulations);
	std::vector<int> values(nbSimulations);
	
	createStatusFile(nbSimulations);
	
	double average = 0;
	double ecartType = 0;
	double esperanceReelle = pow(4,7);
	 
	std::ofstream resultFile;
	resultFile.open("q5_Gattaca.txt", std::ios_base::app);
	
	for (int i=0; i<nbSimulations; i++)
	{
		std::stringstream ss;
        ss << "status" << i+1 << ".txt";
		threads[i]= std::thread(threadGattaca, ss.str(), &(values[i]));
	}
	for(int i=0;i<nbSimulations;++i)
	       threads[i].join();
	for(int i=0;i<nbSimulations;++i)
	{
		resultFile << "Number of draws to get gattaca : " << values[i] << std::endl;
		average += values[i];
		ecartType += (values[i]-esperanceReelle)*(values[i]-esperanceReelle);
	}
	resultFile << std::endl << "Moyenne réelle: " << esperanceReelle << std::endl;
	resultFile << "Moyenne constatée: " << average/nbSimulations << std::endl;
	resultFile << "Ecart Type : " << sqrt(ecartType/nbSimulations) << std::endl;
	
	resultFile.close();
}


int main (int argc, char ** argv)
{
	averageGattaca(40);

	trySentence("status1.txt", "Le hasard n'ecrit pas de programme."); //sans accent

	return 0;
}


