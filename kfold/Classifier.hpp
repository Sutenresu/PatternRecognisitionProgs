#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

#include "Data.hpp"

class Classifier{
private:
    std::vector<Data> dataVector;
    int columns;
    int lines;

    void convertHeader(std::string text, int& lines, int& columns);
    Data convertLineToData(std::string text, int columns);

public:
    Classifier(std::string filePath);
    void kFoldAlgorithm(int kFolds, int kNeighbors, bool normalize);
    int kNeighborsAlgorithm(std::vector<float> data, int kNeighbors, int start, int range);
    std::vector<Data> ZScore();
    void ShowData();
};


