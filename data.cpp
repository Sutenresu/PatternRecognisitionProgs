#include "data.hpp"

Data::Data( std::vector<float> atributesInput, int classInput )
{
    Data::classNumber = classInput;
    Data::atributes = atributesInput;
}

int Data::getClassNumber( void )
{
    return Data::classNumber;
}

std::vector<float> Data::getAtributes( void )
{
    return Data::atributes;
}

void Data::setAtributes( std::vector<float> atributesInput )
{
    Data::atributes = atributesInput;
}

void Data::setClassNumber( int classInput )
{
    Data::classNumber = classInput;
}

void Data::printData( void )
{
    std::cout << "Atributes: ";
    for( int i = 0; i < Data::atributes.size(); i++)
        std::cout << Data::atributes[ i ] << " ";
    std::cout << " - Class: " << Data::classNumber << std::endl;
    // std::cout << Data::classNumber << std::endl;

}
