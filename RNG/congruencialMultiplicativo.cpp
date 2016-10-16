#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <fstream>

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

struct params{
    long X0;
    long a;
    long m;
};

std::vector<params> readData(std::ifstream & _file)
{
    int c = 0;
    std::vector<params> data;
    if(_file.is_open())
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        for( std::string line; getline( _file, line ); )
        {
            if(c < 1)
            {
                getline( _file, line );
                c++;
            }
            long X0, a, m;
            std::istringstream is(line);
            is >> X0 >> a >> m;

            struct params pars;
            pars.X0 = X0;
            pars.a = a;
            pars.m = m;

            data.push_back(pars);            
        }
    }
    else
        std::cout << "File not opened" << std::endl;
    return data;
}

long getRandom(long X0, long a, long m)
{
    return (a * X0) % m;
}

void multCongruential(long X0, long a, long m)
{
    std::vector<long> history;
    int cycle = 1;
    int tail = 1;
    long Xn = getRandom(X0, a, m);    
    history.push_back(Xn);    

    bool done = false;    


    std::cout << "Op" << "\t\t" << "Entero" << "\t\t" << "Xn" << "\t\t" << "Random" << std::endl;
    std::cout << (float)(a * X0)/(float)m << "\t\t" << floor((float)(a * X0)/(float)m) << "\t\t" << Xn << "\t\t" << (float)Xn / (float)m << std::endl;
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
            std::cout << (float)(a * history.back())/(float)m << "\t\t" << floor((float)(a * history.back())/(float)m) << "\t\t" << Xn << "\t\t" << (float)Xn / (float)m << std::endl;
            history.push_back(Xn);
        }
    }
    std::cout << "Cola: " << tail << std::endl;
    std::cout << "Ciclo: " << cycle << std::endl;
    std::cout << "Periodo: " << tail + cycle << std::endl;
}

int main(int argc, char * argv[])
{
    std::cout.precision(5);
    if(argc > 1)
    {
        std::ifstream file;
        std::string path = argv[1];
        openFile(file, path);
        std::vector<params> values = readData(file);
        for (auto pars : values)
        {
            multCongruential(pars.X0, pars.a, pars.m);
            std::cout << "====================================" << std::endl;
        }
        file.close();
    }
    else
    {
        long X0, a, m;   
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

        multCongruential(X0, a, m);
    }   
}