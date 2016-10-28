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

double getP0(double lambda, double miu, int N)
{
    double sum = 0;
    for(int i = 0; i <= N; i++)
    {
        sum += (double)(factorial(N)/factorial(N-i))*pow((double)(lambda/miu),i);
    }
    return (double)(1 / sum);
}

double getLq(double lambda, double miu, double N, double P0)
{
    return N - ((double)(lambda + miu)/lambda)*(1-P0);
}

double getL(double Lq, double P0)
{
    return Lq + (1 - P0);
}

double getWq(double lambda, double Lq, double N, double L)
{
    return (double)Lq / ((N - L)*lambda);
}

double getW(double Wq, double miu)
{
    return Wq + (double)(1/miu);
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

double getPk(double lambda, double miu, double N, double P0, double n)
{
    double left = factorial(N)/factorial(N-n);
    double right = pow((lambda/miu),n);
    return left * right * P0;
}

int main(int argc, char * argv[])
{ 
    /*TODO*/
    std::cout << "Este programa maneja colas M|M|1 con usuarios finitos" << std::endl;
    int choice;
    std::cout << "1) Sin costos" << std::endl;
    std::cout << "2) Con costos" << std::endl;
    std::cout << "Ingresa la opción: ";
    while(!(std::cin >> choice) || choice > 2){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    double lambda, miu, N, Cs, Cw, k;
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
    std::cout << "Ingresa N: ";
    while(!(std::cin >> N)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    std::cout << "Ingresa k: ";
    while(!(std::cin >> k)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    double P0 = getP0(lambda, miu, N);
    double Lq = getLq(lambda, miu, N, P0);
    double L = getL(Lq, P0);
    double Wq = getWq(lambda, Lq, N, L);
    double W = getW(Wq, miu);   
    double Pk = getPk(lambda, miu, N, P0, k);         
    std::cout << "P0: " << P0 << std::endl;
    std::cout << "m*miu > lambda" << std::endl;
    std::cout << "Probability that the system is empty" << std::endl;   
    std::cout << "Lq: " << Lq << std::endl;      
    std::cout << "Average length of the queue" << std::endl; 
    std::cout << "L: " << L << std::endl; 
    std::cout << "Average number of units in the system" << std::endl;  
    std::cout << "Wq: " << Wq << std::endl;      
    std::cout << "Average time in the queue" << std::endl; 
    std::cout << "W: " << W << std::endl;      
    std::cout << "Average time in the system" << std::endl; 
    std::cout << "Pk: " << Pk << std::endl;      
    std::cout << "Probability of n units in the system" << std::endl;         

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
        
        double serviceCost = getTotalServiceCost(1, Cs);
        double waitingCostSys = getWaitingCostSys(lambda, W, Cw);
        double waitingCostQueue = getWaitingCostQueue(lambda, Wq, Cw);
        std::cout << "Service Cost: " << serviceCost << std::endl;
        std::cout << "Waiting Cost System: " << waitingCostSys << std::endl;
        std::cout << "Waiting Cost Queue: " << waitingCostQueue << std::endl;
        std::cout << "Total Queue Cost: " << getTotalQueueCost(serviceCost, waitingCostQueue) << std::endl;
        std::cout << "Total System Cost: " << getTotalSysCost(serviceCost, waitingCostSys) << std::endl;
    }
}