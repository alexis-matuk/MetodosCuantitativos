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

void middleSquare(long dq)
{
    bool repeating = false;
    bool keep = true;
    vector<long> numbers;
    int length = to_string(dq).length();
    numbers.push_back(dq);
    std::string number;
    int first;
    int periodo = 0;
    int longitudc = 0;
    int ciclo = 1;
    int cont = 0;
    std::cout << dq << std::endl;    
    while(keep)
    {
        dq *= dq;
        number = to_string(dq);
        while(number.length() < length)
        {
            number.insert(0, "0");
        }
        if(number.length() % 2)
        {
            number.insert(0, "0");
        }
        while(number.length() > length)
        {            
            number = number.substr(1, number.length());
            number = number.substr(0, number.length()-1);
        }
        std::cout << number << std::endl;
        dq = stol(number);        
        if(find(numbers.begin(), numbers.end(), dq) != numbers.end())
        {
            if(!repeating)
            {
                first = dq;
                periodo = numbers.size();
            }
            else
            {
                if(dq == first)
                {
                    keep = false;

                }
                else{
                    ciclo++;
                }
            }
            repeating = true;
        }
        else
        {
            if(repeating)
            {
                repeating = false;
                ciclo = 1;
            }
        }
        numbers.push_back(dq);
    }
    longitudc = periodo - ciclo;
    cout << "longitud de cola: " << longitudc << endl;
    cout << "periodo: " << periodo << endl;
    cout << "ciclo: " << ciclo << endl;
}

int main(int argc, char * argv[])
{    
    if(argc > 1)
    {
        std::ifstream file;
        std::string path = argv[1];
        openFile(file, path);
        std::vector<long> values = readData(file);
        for (auto seed : values)
        {
            middleSquare(seed);
            std::cout << "====================================" << std::endl;
        }
        file.close();
    }
    else
    {
        long dq;
        std::cout << "Ingresa la semilla: ";
        while(!(std::cin >> dq)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
    middleSquare(dq);
    }    
    return 0;
}
