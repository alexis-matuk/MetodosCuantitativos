#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
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

double getLq(double lambda, double miu)
{
    double upper = pow(lambda,2);
    double lower = (2 * miu)*(miu - lambda);
    return upper/lower;
}

double getL(double lambda, double miu, double Lq)
{
    return Lq + (lambda/miu);
}

double getWq(double lambda, double miu)
{
    double upper = lambda;
    double lower = (2*miu)*(miu - lambda);  
    return upper/lower;  
}

double getW(double lambda, double miu, double Wq)
{
    return Wq + (1/miu);
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

int main(int argc, char * argv[])
{ 
    //lambda
    //miu

    //simular multiples trabajadores con un servidor
    //Simular multiples servidores

    int choice;
    std::cout << "1) Sin costos" << std::endl;
    std::cout << "2) Con costos" << std::endl;
    std::cout << "Ingresa la opción: ";
    while(!(std::cin >> choice) || choice > 2){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    double lambda, miu, m, Cs, Cw;
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
    std::cout << "Ingresa m: ";
    while(!(std::cin >> m)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    
    double Lq = getLq(lambda, miu);    
    double Wq = getWq(lambda, miu);    
    double L = getL(lambda, miu, Lq);
    double W = getW(lambda, miu, Wq);
    
    std::cout << "Lq: " << Lq << std::endl;      
    std::cout << "Average length of queue" << std::endl; 
    std::cout << "L: " << L << std::endl; 
    std::cout << "Average number of customers or units in the system" << std::endl; 
    std::cout << "Wq: " << Wq << std::endl;      
    std::cout << "Average waiting time in queue" << std::endl;     
    std::cout << "W: " << W << std::endl;      
    std::cout << "Average waiting time in the system" << std::endl; 
    

    if(choice != 1)
    {
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
        double serviceCost = getTotalServiceCost(m, Cs);
        double waitingCostSys = getWaitingCostSys(lambda, W, Cw);
        double waitingCostQueue = getWaitingCostQueue(lambda, Wq, Cw);
        std::cout << "Service Cost: " << serviceCost << std::endl;
        std::cout << "Waiting Cost System: " << waitingCostSys << std::endl;
        std::cout << "Waiting Cost Queue: " << waitingCostQueue << std::endl;
        std::cout << "Total Queue Cost: " << getTotalQueueCost(serviceCost, waitingCostQueue) << std::endl;
        std::cout << "Total System Cost: " << getTotalSysCost(serviceCost, waitingCostSys) << std::endl;
    }
}