#include "Data.hpp"
#include <stdio.h>

Data::Data(){}

Data::Data(int nNumbers){
    this->numbers = std::vector<float>(nNumbers);
}

Data::Data(std::vector<float> numbers, int dataClass)
{
    this->numbers = numbers;
    this->dataClass = dataClass;
}

void Data::Show()
{
    for(int i = 0; i < numbers.size(); i++)
        printf("%.2f\t", numbers[i]);

    std::cout << " -> " << dataClass << std::endl;
}
