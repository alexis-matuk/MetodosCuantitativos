#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 
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

std::vector<double> readData(std::ifstream & _file)
{
    std::vector<double> data;
    if(_file.is_open())
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        for( std::string line; getline( _file, line ); )
        {
            data.push_back(stod(line));            
        }
    }
    else
        std::cout << "File not opened" << std::endl;
    return data;
}

//Hacerlo de pointers para meter NULL

void fillDplus(std::vector<double> & dPlus, std::vector<double> data, int length)
{
    std::cout << "=======" << std::endl;
    for(int i = 0; i < length; i++)
    {
        double upper = i + 1;
        double lower = length;
        double res = (upper/lower) - data[i];
        if(res > 0)
            dPlus.push_back(res);
    }
}

void fillDMinus(std::vector<double> & dMinus, std::vector<double> data, int length)
{
    for(int i = 0; i < length; i++)
    {
        double upper = i;
        double lower = length;
        double res = data[i] - (upper/lower);
        if(res > 0)
            dMinus.push_back(res);
    }
}


int main(int argc, char * argv[])
{    
    std::ifstream file;
    std::string path = argv[1];
    openFile(file, path);
    std::vector<double> data = readData(file);
    std::sort(data.begin(), data.end());
    std::vector<double> dPlus, dMinus; 
    int length = data.size();   
    fillDplus(dPlus, data, length);
    fillDMinus(dMinus, data, length);    
    double maxDPlus, maxDMinus;
    maxDPlus = *std::max_element(dPlus.begin(), dPlus.end());
    maxDMinus = *std::max_element(dMinus.begin(), dMinus.end());
    std::cout << "Max D+: " << maxDPlus << std::endl;
    std::cout << "Max D-: " << maxDMinus << std::endl;
    std::cout << "Max{D+, D-}: " << std::max(maxDMinus, maxDPlus) << std::endl;
    std::cout << "Check table for D < Da to see if it follows a uniform distribution" << std::endl;
    return 0;
}
