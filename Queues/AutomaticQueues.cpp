#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;

void openFile(std::ifstream & _file, std::string route)
{
    try
    {
        if (std::ifstream(route))
        {
            std::cout << "File found... opening " << std::endl;
            _file.open(route);
            if(_file.fail())
            {
                std::cout << "Error reading file... aborting" << std::endl;
                throw 2;
            }
        }
        else
        {
            std::cout << "File not found... Aborting" << std::endl;
            throw 1;
        }
    }
    catch (int e)
    {
        std::cerr << "Error reading data... Exception " << e << " caught" << std::endl;
    }
}

std::vector<long> readData(std::ifstream & _file)
{
    std::vector<long> data;
    if(_file.is_open())
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        for( std::string line; getline( _file, line ); )
        {
            data.push_back(stol(line));            
        }
    }
    else
        std::cout << "File not opened" << std::endl;
    return data;
}

double factorial(double number)
{
    double res = 1;
    for(int i = 1; i <= number; i++)
    {
        res *= i;
    }
    return res;
}

double getP0(double lambda, double miu, int m)
{
    double left = 0;
    double res = 0;
    for(int i = 0; i < m; i++)
    {
        left += (double)(1/factorial(i)) * pow((double)(lambda/miu), i);
    }    
    res = left + ((double)(1/factorial(m)) * pow((double)(lambda/miu), m) * (double)((m * miu)/(m*miu - lambda)));
    return 1/res;
}

double getL(double lambda, double miu, double m, double P0)
{
    double upper = lambda * miu *(pow((lambda/miu),m));
    double lower = factorial(m - 1) * pow((m*miu - lambda),2);
    return ((upper/lower)*P0)+(lambda/miu);
}

double getW(double L, double lambda)
{
    return L / lambda;
}

double getLq(double L, double lambda, double miu)
{
    return L - (lambda/miu);
}

double getWq(double Lq, double lambda)
{
    return Lq/lambda;
}

double getRho(double lambda, double miu, double m)
{
    return lambda/(m * miu);
}


double getTotalServiceCost(double m, double Cs)
{
    return m * Cs;
}

double getWaitingCostSys(double lambda, double W, double Cw)
{
    return (lambda * W)*Cw;
}

double getWaitingCostQueue(double lambda, double Wq, double Cw)
{
    return (lambda * Wq)*Cw;
}

double getTotalSysCost(double serviceCost, double waitingCostSys)
{
    return serviceCost + waitingCostSys;
}

double getTotalQueueCost(double serviceCost, double waitingCostQueue)
{
    return serviceCost + waitingCostQueue;
}

struct Result{
    double P0;
    double L;
    double W;
    double Lq;
    double Wq;
    double Rho;
    double serviceCost;
    double waitingCostSys;
    double waitingCostQueue;
    double totalQueueCost;
    double totalSysCost;        
};

std::ostream& dump(std::ostream &o, const struct Result& res)
{    
    o << "========================" << std::endl;
    o << "P0: " << res.P0 << std::endl;
    o << "L: " << res.L << std::endl;
    o << "W: " << res.W << std::endl;
    o << "Lq: " << res.Lq << std::endl;
    o << "Wq: " << res.Wq << std::endl;
    o << "Rho: " << res.Rho << std::endl;
    o << "serviceCost: " << res.serviceCost << std::endl;
    o << "waitingCostSys: " << res.waitingCostSys << std::endl;
    o << "waitingCostQueue: " << res.waitingCostQueue << std::endl;
    o << "totalQueueCost: " << res.totalQueueCost << std::endl;
    o << "totalSysCost: " << res.totalSysCost << std::endl;
    return o;
}

std::ostream& operator << (std::ostream &o, const Result &a){
  return dump(o,a);
}

Result performSimulation(double lambda, double miu, double m, double Cs, double Cw)
{
    double P0 = getP0(lambda, miu, m);
    double L = getL(lambda, miu, m, P0);
    double W = getW(L, lambda);
    double Lq = getLq(L, lambda, miu);
    double Wq = getWq(Lq, lambda);
    double Rho = getRho(lambda, miu, m);     
    double serviceCost = getTotalServiceCost(m, Cs);
    double waitingCostSys = getWaitingCostSys(lambda, W, Cw);
    double waitingCostQueue = getWaitingCostQueue(lambda, Wq, Cw);
    double totalQueueCost = getTotalQueueCost(serviceCost, waitingCostQueue);
    double totalSysCost = getTotalSysCost(serviceCost, waitingCostSys);
    struct Result res;
    res.P0 = P0;
    res.L = L;
    res.W = W;
    res.Lq = Lq;
    res.Wq = Wq;
    res.Rho = Rho;
    res.serviceCost = serviceCost;
    res.waitingCostSys = waitingCostSys;
    res.waitingCostQueue = waitingCostQueue;
    res.totalQueueCost = totalQueueCost;
    res.totalSysCost = totalSysCost;
    return res;
}

bool AllComparisons(Result newSim, Result minSerial, std::vector<std::function<bool(Result,Result)>> comparisonFunctions)
{	
	for(int i = 0; i < comparisonFunctions.size(); i++)
	{
		if(!comparisonFunctions[i](newSim, minSerial))
			return false;
	}
	return true;
}

Result OptimizeLinear(double lambda, double miu, double Cs, double Cw, std::vector<std::function<bool(Result,Result)>> comparisonFunctions)
{
    struct Result minSerial = performSimulation(lambda, miu, 1, Cs, Cw);
    int i = 2;
    struct Result newSim = performSimulation(lambda, miu * i, 1, Cs, Cw);
    while(AllComparisons(newSim, minSerial, comparisonFunctions))
    {
        minSerial = newSim;
        i++;
        newSim = performSimulation(lambda, miu * i, 1, Cs, Cw);        
    }
    std::cout << minSerial << std::endl;
    std::cout << "miu: " << miu * i << std::endl;
    std::cout << "m: 1" << std::endl; 
    return minSerial;
}

Result OptimizeParallel(double lambda, double miu, double Cs, double Cw, std::vector<std::function<bool(Result,Result)>> comparisonFunctions)
{
    int i = 0;
    struct Result minSerial = performSimulation(lambda, miu, ++i, Cs, Cw);    
    struct Result newSim = performSimulation(lambda, miu, ++i, Cs, Cw);
    while(AllComparisons(newSim, minSerial, comparisonFunctions))
    {
        minSerial = newSim;
        newSim = performSimulation(lambda, miu, ++i, Cs, Cw);        
    }
    std::cout << minSerial << std::endl;
    std::cout << "miu: " << miu  << std::endl;
    std::cout << "m: " << i << std::endl; 
    return minSerial;
}

std::function<bool(Result, Result)> GetComparer(int choice)
{
    std::function<bool(Result,Result)> res;
    std::vector<std::function<bool(Result,Result)>> funcs = {
    	[](Result a, Result b){return a.P0 < b.P0;},
    	[](Result a, Result b){return a.L < b.L;},
    	[](Result a, Result b){return a.W < b.W;},
    	[](Result a, Result b){return a.Lq < b.Lq;},
    	[](Result a, Result b){return a.Wq < b.Wq;},
    	[](Result a, Result b){return a.Rho < b.Rho;},
    	[](Result a, Result b){return a.waitingCostSys < b.waitingCostSys;},
    	[](Result a, Result b){return a.waitingCostQueue < b.waitingCostQueue;},
    	[](Result a, Result b){return a.totalQueueCost < b.totalQueueCost;},
    	[](Result a, Result b){return a.totalSysCost < b.totalSysCost;}
	};
    return funcs[choice];
}

int main(int argc, char * argv[])
{ 
    //lambda
    //miu

    //simular multiples trabajadores con un servidor
    //Simular multiples servidores    

    double lambda, miu, Cs, Cw = 0;

    std::cout << "Ingresa lambda: ";
    while(!(std::cin >> lambda)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    std::cout << "Ingresa miu: ";
    while(!(std::cin >> miu)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    //con costos
    std::cout << "Ingresa Cs(service cost): ";
    while(!(std::cin >> Cs)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    std::cout << "Ingresa Cw(cost of waiting): ";
    while(!(std::cin >> Cw)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    struct Result minSerial;
    struct Result minParallel;

    int choice;
    std::cout << "Variable a optimizar: " << std::endl;
    std::cout << "1) P0" << std::endl;
    std::cout << "2) L" << std::endl;
    std::cout << "3) W" << std::endl;
    std::cout << "4) Lq" << std::endl;
    std::cout << "5) Wq" << std::endl;
    std::cout << "6) Rho" << std::endl;
    std::cout << "7) Waiting Cost System" << std::endl;
    std::cout << "8) Waiting Cost Queue" << std::endl;
    std::cout << "9) Total Queue Cost" << std::endl;
    std::cout << "10) Total System Cost" << std::endl;

    std::vector<std::function<bool(Result,Result)>> comparisonFunctions;
	std::cin.ignore();
	std::string line;
	std::getline(std::cin, line);
	std::istringstream input(line);
	std::vector<int> result;
	int value;
	while (input >> value)
	{
	    comparisonFunctions.push_back(GetComparer(value));
	}
    std::cout << "Optimized Linear Result" << std::endl;
    OptimizeLinear(lambda, miu, Cs, Cw, comparisonFunctions); 
    std::cout << "Optimized Parallel Result" << std::endl;
    OptimizeParallel(lambda, miu, Cs, Cw, comparisonFunctions); 
}