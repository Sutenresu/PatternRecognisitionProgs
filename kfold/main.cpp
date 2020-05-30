#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

#include "Classifier.hpp"

using namespace std;

int main(){

    try{
        Classifier classifier("data2.txt");
        classifier.kFoldAlgorithm(10,3, false);
        //classifier.ShowData();
    }catch(invalid_argument ex){
        cerr << ex.what() << endl;
    }

    return 0;
}

