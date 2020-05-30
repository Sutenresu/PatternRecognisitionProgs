#include <vector>
#include <iostream>
#include <ctime>      
#include "data.hpp"
#include "classifier.hpp"
//#define DEBUG

int main(int argc, char *argv[])
{
    // std::vector<float> data1 (4, 100.0);
    Classifier teste;
    // teste.printAllData();
    // Data dado(teste, 2);
    // dado.printData();
    // std::cout << "\nKNN + KFold para reconhecimento de padrão de dados em arquivos txt com o seguinte formato" << std::endl;
    // std::cout << "\n150 4\n5.1 3.5 1.4 0.2 1\n4.9 3.0 1.4 0.2 1\n4.7 3.2 1.3 0.2 1\n" <<
    //             "4.6 3.1 1.5 0.2 1\n5.0 3.6 1.4 0.2 1"<< std::endl;
    // std::string nameFile;
    // std::cout << "\nInforme o nome do arquivo a ser utilizado (exemplo = dados.txt ): ";
    // std::cin >> nameFile;
    
    // #ifdef DEBUG
    //     std::cout << "\nArquivo informado: " << nameFile << std::endl;
    // #endif
    
    // char zscore = 'a';
    // while( zscore!= 'y' && zscore!= 'n')
    // {
    //     std::cout << "Deseja usar Z-Score para normalizar os dados? (y/n) ";
    //     std::cin >> zscore;
    // }
    // if (zscore =='n')
    //     std::cout << "\nNão será normalizado usando ZScore." << std::endl;
    // else
    //     std::cout << "\nSerá normalizado usando ZScore." << std::endl;

    // std::ifstream streamFile ( nameFile.c_str() );
    // std::string lineBuffer;
    // int numberSamples, numberAttributes;
    // if ( streamFile.is_open() )
    // {
    //     //A primeira linha tem o numero de amostras e o numero de atributos por amostra
    //     std::getline (streamFile,lineBuffer);
    //     std::string::size_type size;     // alias of size_t
        
    //     numberSamples = std::stoi( lineBuffer,&size );
    //     numberAttributes = std::stoi ( lineBuffer.substr(size) );
    // }
    // streamFile.close();
    // std::cout << "\nNúmero de amostras: " << numberSamples << std::endl;

    // int knn = -1, kfold = -1;
    // while( knn < 0 || knn > numberSamples)
    // {
    //     std::cout << "Qual o número de vizinhos desejado para o cálculo do knn? ";
    //     std::cin >> knn;
    // }

    // #ifdef DEBUG
    //     std::cout << "\nknn: " << knn << std::endl;
    // #endif

    // while( kfold < 0 || kfold > numberSamples)
    // {
    //     std::cout << "Qual o número de partições dos dados para o kfold? ";
    //     std::cin >> kfold;
    // }

    // #ifdef DEBUG
    //     std::cout << "\nkfold: " << kfold << std::endl;
    // #endif

    // Classifier dados( nameFile, knn, kfold, zscore=='y'? NORMALIZE_ON:NORMALIZE_OFF );
    // dados.printAllData();
    return 0;
}
