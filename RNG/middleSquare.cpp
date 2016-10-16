#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main(int argc, char * argv[])
{
    bool repeating = false;
    bool keep = true;
    vector<long> numbers;
    long dq;
    std::cout << "Ingresa la semilla: ";
    while(!(std::cin >> dq)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida, intenta otra vez: ";
    }
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
    return 0;
}
