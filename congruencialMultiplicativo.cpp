#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
using namespace std;

long getRandom(long X0, long a, long m)
{
	return (a * X0) % m;
}

int main(int argc, char * argv[])
{
	long X0, a, m;
	std::vector<long> history;
	std::cout << "Ingresa X0: ";
	while(!(std::cin >> X0)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    std::cout << "Ingresa a: ";
	while(!(std::cin >> a)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    std::cout << "Ingresa m: ";
	while(!(std::cin >> m)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    int cycle = 1;
    int tail = 1;
    long Xn = getRandom(X0, a, m);    
    history.push_back(Xn);    

    bool done = false;    
    std::cout << "Op" << "	" << "Entero" << "	" << "Xn" << "	" << "Random" << std::endl;
    std::cout << (float)(a * X0)/(float)m << "	" << floor((float)(a * X0)/(float)m) << "	" << Xn << "	" << (float)Xn / (float)m << std::endl;
    while(!done)
    {    	    	
    	Xn = getRandom(history.back(), a, m);  	
    	if(find(history.begin(), history.end(), Xn) != history.end())
    	{
    		std::vector<long> cycleHistory;
    		cycleHistory.push_back(Xn);    		
    		long needle = getRandom(cycleHistory.back(), a, m);
    		cycleHistory.push_back(needle);
    		while(needle != Xn)
    		{
    			needle = getRandom(cycleHistory.back(), a, m);
    			cycle++;
    			cycleHistory.push_back(needle);
    		}
    		done = true;
    	}
    	else
    	{
    		tail++;
    		std::cout << (float)(a * history.back())/(float)m << "	" << floor((float)(a * history.back())/(float)m) << "	" << Xn << "	" << (float)Xn / (float)m << std::endl;
    		history.push_back(Xn);
    	}
    }
    std::cout << "Cola: " << tail << std::endl;
    std::cout << "Ciclo: " << cycle << std::endl;
    std::cout << "Periodo: " << tail + cycle << std::endl;
}