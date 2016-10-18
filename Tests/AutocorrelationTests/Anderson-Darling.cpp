#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>

class Pair{
public:
    float min;
    float max;
};

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
            double x;
            std::istringstream i(line);
            i >> x;
            data.push_back(x);
        }
    }
    else
        std::cout << "File not opened" << std::endl;
    return data;
}

int getM(int N, int i, int l)
{
    int M = 0;
    int calc = i + (M + 1) * l;
    while(calc <= N)
    {        
        M++;
        calc = i + (M + 1) * l;
    }
    M--;    
    return M;
}

double getRho(std::vector<double> data, int M, int start, int l)
{
    double sum = 0;
    std::cout << "1/(" << M << " + 1) * ["; 
    for(int i = 0; i <= M; i++)
    {
        sum += data[start + (i*l) - 1] * data[start + ((i+1)*l) - 1];
        std::cout << "(" << data[start + (i*l) - 1] << "*" << data[start + ((i+1)*l) - 1] << ")";
    }    
    std::cout << "] - 0.25" << std::endl;    
    return (double) ((double)1/(double)(M+1)) * sum - 0.25;
}

int main(int argc, char * argv[])
{
    std::cout.precision(4);
    std::ifstream file;
    std::string path = argv[1];
    openFile(file, path);
    std::vector<double> data = readData(file);
    int N = data.size();
    int start = 0;
    std::cout << "Ingresa i: ";
    while(!(std::cin >> start)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }

    int l;
    std::cout << "Ingresa l: ";
    while(!(std::cin >> l)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    int M = getM(N, start, l);
    double rho = getRho(data, M, start, l);
    std::cout << "Rho: " << rho << std::endl;
    double lower = sqrt((13 * M) + 7)/(12*(M+1));
    double Z0 = rho / lower;
    std::cout << "Z0: " << Z0 << std::endl;

}