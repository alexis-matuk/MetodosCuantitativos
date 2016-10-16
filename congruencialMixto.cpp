#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

long getRandom(long X0, long a, long c, long m)
{
	return ((a * X0) + c) % m;
}

int main(int argc, char * argv[])
{
	long X0, a, c, m;
	std::vector<long> history;
	std::vector<float> randomHistory;
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
    std::cout << "Ingresa c: ";
	while(!(std::cin >> c)){
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
    long Xn = getRandom(X0, a, c, m);
    float random = Xn / m;
    history.push_back(Xn);
    randomHistory.push_back(random);
    tail ++;

    bool done = false;    
    std::cout << "Xn" << "	" << "Random" << std::endl;
    while(!done)
    {    	    	
    	Xn = getRandom(history[history.size()], a, c, m);
    	random = Xn / m;    	
    	if(find(history.begin(), history.end(), Xn) != history.end())
    	{
    		long needle = getRandom(X0, a, c, m);
    		std::vector<long> cycleHistory;
    		cycleHistory.push_back(Xn);    		
    		while(needle != cycleHistory[0])
    		{
    			needle = getRandom(cycleHistory[cycleHistory.size()], a, c, m);
    			cycle++;
    		}
    		done = true;
    	}
    	else
    	{
    		history.push_back(Xn);
    		randomHistory.push_back(random);
    		tail++;
    		std::cout << Xn << "	" << random << std::endl;
    	}
    }
    std::cout << "Cola: " << tail << std::endl;
    std::cout << "Ciclo: " << cycle << std::endl;
    std::cout << "Periodo: " << tail + cycle << std::endl;
}