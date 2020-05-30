#ifndef DATA_H
#define DATA_H

#include <vector>
#include <cstdlib>
#include <iostream>

class Data
{

public:

Data( std::vector<float> atributesInput, int classInput );
std::vector<float> getAtributes( void );
int getClassNumber( void );
void setAtributes( std::vector<float> atributesInput );
void setClassNumber( int classInput );
void printData( void );
//float getAtribute(int atributeNumber);

private:

std::vector<float> atributes;
int classNumber;


};

#endif // DATA_H