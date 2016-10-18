#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
using namespace std;

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

void printData(std::vector<Pair> ranges, std::vector<int> frequencies)
{
	std::cout << "Classes" << "\t\t" << "Oi" << "\t" << "Ei" << "\t\t" << "Oi - Ei" << "\t\t" << "(Oi - Ei)^2" << "\t" << "(Oi - Ei)^2/Ei" <<std::endl;
	double expected = (double)(1/(double)ranges.size())*100;
	double total = 0;
    for(int i = 0; i < ranges.size(); i++)
    {
    	total += (double)((frequencies[i] - expected)*(frequencies[i] - expected))/expected;
        std::cout << ranges[i].min << " - " << ranges[i].max << "\t" << frequencies[i] << "\t" << expected << "\t\t" << frequencies[i] - expected << "\t\t" << (frequencies[i] - expected)*(frequencies[i] - expected) << "\t\t" << (double)((frequencies[i] - expected)*(frequencies[i] - expected))/expected << std::endl;
    }
    std::cout << "Total: " << total << std::endl;
}

bool areSame(double a, double b)
{
    return fabs(a - b) < 0.0001;
}

int findRange(std::vector<Pair> ranges, double number)
{
    for(int i = 0; i < ranges.size(); i++)
    {
        if((number > ranges[i].min && number < ranges[i].max) || areSame(number, ranges[i].min) || areSame(number, ranges[i].max))
        {
            return i;
        }
    }
    return 0;
}

bool incompleteClasses(std::vector<int> frequencies, int & pos)
{
    for(int i = 0; i < frequencies.size(); i++)
    {
        pos = i;
        if (frequencies[i] < 5) return true;
    }
    pos = 0;    
    return false;
}

void mergeClasses(std::vector<Pair> & ranges, std::vector<int> & frequencies)
{
    int pos;
    while(incompleteClasses(frequencies, pos))
    {
        if(pos == 0)
        {
            Pair newPair;
            newPair.min = ranges[pos].min;
            newPair.max = ranges[pos + 1].max;
            int newFreq = frequencies[pos] + frequencies[pos + 1];
            ranges[pos] = newPair;
            frequencies[pos] = newFreq;
            ranges.erase(ranges.begin() + pos + 1);
            frequencies.erase(frequencies.begin() + pos + 1);
        }
        else if(pos == frequencies.size())
        {
            Pair newPair;
            newPair.min = ranges[pos - 1].min;
            newPair.max = ranges[pos].max;
            int newFreq = frequencies[pos] + frequencies[pos - 1];
            ranges[pos] = newPair;
            frequencies[pos] = newFreq;
            ranges.erase(ranges.begin() + pos - 1);
            frequencies.erase(frequencies.begin() + pos - 1);
        }
        else
        {            
            if(frequencies[pos - 1] < frequencies[pos + 1])
            {
                //Con el de arriba
                Pair newPair;
                newPair.min = ranges[pos - 1].min;
                newPair.max = ranges[pos].max;
                int newFreq = frequencies[pos] + frequencies[pos - 1];
                ranges[pos] = newPair;
                frequencies[pos] = newFreq;
                ranges.erase(ranges.begin() + pos - 1);
                frequencies.erase(frequencies.begin() + pos - 1);
            }
            else
            {
                //Con el de abajo
                Pair newPair;
                newPair.min = ranges[pos].min;
                newPair.max = ranges[pos + 1].max;
                int newFreq = frequencies[pos] + frequencies[pos + 1];
                ranges[pos] = newPair;
                frequencies[pos] = newFreq;
                ranges.erase(ranges.begin() + pos + 1);
                frequencies.erase(frequencies.begin() + pos + 1);
            }
        }
    }
}

int main(int argc, char * argv[])
{
	std::cout.precision(4);
    std::ifstream file;
    std::string path = argv[1];
    openFile(file, path);
    std::vector<double> data = readData(file);
    int length = data.size();
    int numberOfClasses =  ceil((double)(log(length) / log(2)));
    double max = *std::max_element(data.begin(), data.end());
    double min = *std::min_element(data.begin(), data.end());
    double interval = (double)(max - min) / (double)numberOfClasses;
    double maxVal = min;    

    std::ostringstream strs;
    strs << interval;
    std::string str = strs.str();
    int dotPos = str.find('.');
    int fracPart = stoi(str.substr(dotPos+1, str.length()));
    str.replace(dotPos+1, str.length(), std::to_string(fracPart-1));
    double sum = stod(str);


    double lastMaxLimit;
    std::vector<Pair> ranges;
    while(maxVal < max)
    {
        Pair newPair;
        newPair.min = maxVal;
        newPair.max = maxVal + sum;
        ranges.push_back(newPair);
        lastMaxLimit = maxVal + sum;
        maxVal += interval;
    }
    while(lastMaxLimit < max)
    {
        maxVal = min;
        ranges.clear();
        std::ostringstream strs;
        strs << interval;
        std::string str = strs.str();
        int dotPos = str.find('.');
        int fracPart = stoi(str.substr(dotPos+1, str.length()));
        str.replace(dotPos+1, str.length(), std::to_string(fracPart+1));
        interval = stod(str);
        sum = interval - maxVal;
        
        while(maxVal < max)
        {
            Pair newPair;
            newPair.min = maxVal;
            newPair.max = maxVal + sum;
            ranges.push_back(newPair);
            lastMaxLimit = maxVal + sum;
            maxVal += interval;
        }
    }
    std::cout << "interval: " << interval << std::endl;
    std::cout << "# classes: " << numberOfClasses << std::endl;
    
    std::vector<int> frequencies(ranges.size(),0);
    
    for(int i = 0; i < data.size(); i++)
    {
        frequencies[findRange(ranges, data[i])] += 1;
    }
    mergeClasses(ranges, frequencies);
    printData(ranges, frequencies);
    return 0;
}