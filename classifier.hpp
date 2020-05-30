#include "data.hpp"
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>

#define STD_FILE "teste_lbp.txt"
#define NORMALIZE_ON 1
#define NORMALIZE_OFF 0
#define MAX_STREAM 50

class Classifier{

public:
Classifier( std::string = STD_FILE, int numberNeighbors = 10,
            int numberFolds = 5, int zscore = NORMALIZE_ON);
void printAllData( void );
void printDataVector( std::vector<Data> src );
void printIntVector( std::vector<int> src );
void printFloatVector( std::vector<float> src );

private:
void readDataFromTxt( std::string fileName );
float kFoldAlgorithm( void );
int kNNAlgorithm( std::vector<Data> training_set, Data valid_data );
void orderDistanceClassVectors( std::vector<int> &classVector, std::vector<float> &distanceVector );
int kNeighbors;
int kFolds;
int numberSamples;
int numberAttributes;
float distanceMeasurer( Data data1, Data data2 );
void normalizingZScore( std::vector<Data> &src );
void sortDataByClass( std::vector<Data> &src );
std::vector<Data> dataSampling;

};