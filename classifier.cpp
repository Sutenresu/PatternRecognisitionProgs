#include "classifier.hpp"
// #define TESTING_FILE_READING
// #define DISTANCE_MEASURING
// #define TESTING_KFOLD
// #define TESTING_SORT
// #define TESTING_KNN
 #define TESTING_DRAW

Classifier::Classifier( std::string fileName, int numberNeighbors,
            int numberFolds, int zscore)
{
    Classifier::kFolds = numberFolds;
    Classifier::kNeighbors = numberNeighbors;
    
    Classifier::readDataFromTxt( fileName );
   
    if ( zscore )
        Classifier::normalizingZScore( Classifier::dataSampling );
    
    float precision = Classifier::kFoldAlgorithm();
    #ifdef DISTANCE_MEASURING
        float dist = Classifier::distanceMeasurer( dataSampling[0], dataSampling[1] );
    #endif
    std::cout << "\nA taxa de precisão obtida foi de " << precision * 100 << "%. " << std::endl;
}

void Classifier::readDataFromTxt( std::string fileName )
{
    std::string lineBuffer;
    int auxCounter;

    std::ifstream streamFile ( fileName.c_str() );

    if ( streamFile.is_open() )
    {
        //A primeira linha tem o numero de amostras e o numero de atributos por amostra
        std::getline (streamFile,lineBuffer);
        std::string::size_type size;     // alias of size_t
        
        Classifier::numberSamples = std::stoi( lineBuffer,&size );
        Classifier::numberAttributes = std::stoi ( lineBuffer.substr(size) );
        std::vector<float> streamFloatValues( Classifier::numberAttributes, 0.0 );
          

        #ifdef TESTING_FILE_READING    
            char ch;
            std::cout << "amostras: " << Classifier::numberSamples <<std::endl;
            std::cout << "atributos: " << Classifier::numberAttributes <<std::endl;
        #endif

        while ( std::getline (streamFile,lineBuffer) )
        {
            streamFloatValues[ 0 ] = std::stof( lineBuffer, &size );

            #ifdef TESTING_FILE_READING
                std::cout << "buffer: " << lineBuffer <<std::endl;
                std::cout << "primeiro: " << streamFloatValues[ 0 ] <<std::endl;
                std::cout << "size: " << size <<std::endl;
            #endif

            for( int i = 1; i < Classifier::numberAttributes; i++)
            {
                lineBuffer = lineBuffer.substr( size );
                streamFloatValues[ i ] = std::stof( lineBuffer, &size );

                #ifdef TESTING_FILE_READING
                    ch = std::getchar();
                    std::cout << "proximo: " << streamFloatValues[ i ] <<std::endl;
                    std::cout << "size: " << size <<std::endl;
                #endif
            }

            #ifdef TESTING_FILE_READING
                std::cout << "classe: " << std::stoi( lineBuffer.substr( size ) ) <<std::endl;
                std::cout << "size: " << size << std::endl;
            #endif

            Data auxData( streamFloatValues , std::stoi( lineBuffer.substr( size ) ) );
            dataSampling.push_back( auxData );
        }

    }
    streamFile.close();
}



float Classifier::kFoldAlgorithm( void )
{
    int elementsFold = Classifier::numberSamples / Classifier::kFolds;
    std::vector<Data> trainingData;
    int calculatedClass;
    int numberSucess = 0;
    float precisionAlgorithm = 0.0;

    #ifdef TESTING_KFOLD
        char ch;
        std::cout << "Número de amostras = " << Classifier::numberSamples <<
            "\nNúmero de partições = " << Classifier::kFolds << "\nNúmero de elementos = " 
            << elementsFold << std::endl;
    #endif

    // #ifdef TESTING_KFOLD
    //     std::cout << "Antes de distribuir melhor os dados" << std::endl;
    //     Classifier::printAllData();
    //     ch = getchar();
    // #endif

    Classifier::sortDataByClass( Classifier::dataSampling );

    // #ifdef TESTING_KFOLD
    //     std::cout << "Depois de distribuir melhor os dados" << std::endl;
    //     Classifier::printAllData();
    //     ch = getchar();
    // #endif

    for( int pointerValid = 0; pointerValid < Classifier::numberSamples; pointerValid+=elementsFold )
    {
       
        trainingData = Classifier::dataSampling;

        // #ifdef TESTING_KFOLD
        //     std::cout << "pointer = " << pointerValid << std::endl;
        //     Classifier::printDataVector( trainingData );
        //     ch = getchar();
        // #endif

        for ( int j = 0; j < elementsFold; j++)
        {
            trainingData.erase( trainingData.begin() + pointerValid);
        }

        // #ifdef TESTING_KFOLD
        //     std::cout << "\nDepois de tirar os dados de validação" << std::endl;
        //     Classifier::printDataVector( trainingData );
        //     ch = getchar();
        // #endif

        //Agora trainingData tem somente os dados de treinamento necessários para executar o knn
        
        for ( int j = 0; j < elementsFold; j++)
        {
            if( (pointerValid + j ) < Classifier::dataSampling.size() )
            {
                #ifdef TESTING_KFOLD
                    std::cout << "Valid_data do momento está na posição " << pointerValid + j << std::endl;
                    std::cout << "o que tem nela ";
                    Classifier::dataSampling[ pointerValid + j ].printData();
                    std::cout << std::endl;
                #endif
                    calculatedClass = kNNAlgorithm( trainingData, Classifier::dataSampling[ pointerValid + j ] );
            
                #ifdef TESTING_KFOLD
                    std::cout << "\nj =" << j << std::endl;
                    std::cout << "Classe Calculada pelo kNN" << calculatedClass << std::endl;
                    std::cout << "Classe verdadeira" << Classifier::dataSampling[ pointerValid + j ].getClassNumber() << std::endl;
                    ch = getchar();
                #endif

                if( calculatedClass == Classifier::dataSampling[ pointerValid + j ].getClassNumber() )
                    numberSucess++;

                #ifdef TESTING_KFOLD
                    std::cout << "\nNúmero de sucessos" << numberSucess << std::endl;
                #endif
            }
        }
    }
    precisionAlgorithm = ( (float) numberSucess ) / Classifier::numberSamples;

    #ifdef TESTING_KFOLD
        std::cout << "\nNúmero de sucessos" << numberSucess << std::endl;
        std::cout << "\nPrecisão" << precisionAlgorithm << std::endl;
    #endif

    return precisionAlgorithm;
}

int Classifier::kNNAlgorithm( std::vector<Data> training_set, Data valid_data )
{
    std::vector<int> classHolder(Classifier::kNeighbors,0);
    std::vector<float> distanceHolder(Classifier::kNeighbors,0.0);
    int classAux = 0;
    float distanceAux = 0.0;

    //Primeiramente, preencho dois vetores com classe e distancia, com os primeiros
    //kvizinhos, já colocando eles em ordem. [0] tem o mais perto
    //a comparação foi feita com o membro passado e o atual, por isso tratei o 0 separadamente
    for( int i = 0; i < Classifier::kNeighbors; i++ )
    {
        if( !i )
        {
            classHolder[i] = training_set[i].getClassNumber();
            distanceHolder[i] = distanceMeasurer( training_set[i] , valid_data );
        }else
        {
            classAux = training_set[i].getClassNumber();
            distanceAux = distanceMeasurer( training_set[i] , valid_data );
            if( distanceAux < distanceHolder[i-1] )
            {
                classHolder[i] = classHolder[i-1];
                distanceHolder[i] = distanceHolder[i-1];
                classHolder[i-1] = classAux;
                distanceHolder[i-1] = distanceAux;
            } else
            {
                classHolder[i] = classAux;
                distanceHolder[i] = distanceAux;
            }
        }
    }

    #ifdef TESTING_KNN
        char ch;
        std::cout << "Número de vizinhos = " << Classifier::kNeighbors << std::endl;
        Classifier::printIntVector( classHolder );
        Classifier::printFloatVector( distanceHolder );
        ch = getchar();
    #endif
    //agora vou varrendo todos os dados de treinamento restante para deixar os melhores kvizinhos
    //guardados. para isso vejo se o que estou analisando atualmente é melhor que o pior. se sim,
    //apago o pior guardo esse e chamo a função que ordena, para tratar separadamente essa questao
    for( int i = Classifier::kNeighbors; i < (int) training_set.size(); i++ )
    {
        
        classAux = training_set[i].getClassNumber();
        distanceAux = distanceMeasurer( training_set[i] , valid_data );
        
        // #ifdef TESTING_KNN
        //     std::cout << "Vizinho atual ["<< i << "]" << std::endl;
        //     std::cout << "Sua classe é " << classAux << std::endl;
        //     std::cout << "Sua distância é " << distanceAux << std::endl;
        //     ch = getchar();
        // #endif

        if( distanceAux < distanceHolder[ ( distanceHolder.size() - 1 ) ] )
        {
            // #ifdef TESTING_KNN    
            //     std::cout << "atualiza" << std::endl;
            // #endif
            classHolder.pop_back();
            distanceHolder.pop_back();
            // #ifdef TESTING_KNN    
            //     std::cout << "apaga" << std::endl;
            // #endif
            classHolder.push_back( classAux );
            distanceHolder.push_back( distanceAux );
            // #ifdef TESTING_KNN  
            //     std::cout << "resultado: " <<std::endl;
            //     Classifier::printIntVector( classHolder );
            //     Classifier::printFloatVector( distanceHolder );
            // #endif
            Classifier::orderDistanceClassVectors( classHolder, distanceHolder );
        }
    }

    #ifdef TESTING_KNN
        std::cout << "Vizinhos mais pertos"<< std::endl;
        Classifier::printIntVector( classHolder );
        Classifier::printFloatVector( distanceHolder );
        ch = getchar();
    #endif

    //por fim, preciso retornar a moda da classe dos kvizinhos selecionados
    //primeiro, ordeno os kvizinhos de acordo com classe
    //como comparo com a da frente, paro em < size-1
    //a partir daqui não tem importância a distância mais, então não preocupo com o distanceHolder mais
    for( int i = 0; i < ( (int) classHolder.size() - 1 ); i++ )
    {
        if( classHolder[i] > classHolder[i+1] )
        {
            classAux = classHolder[i];
            classHolder[i] = classHolder[i+1];
            classHolder[i+1] = classAux;
        }
    }

    int classMost = -1, classQuantMost = 0;
    //moda, quantidade
    int classPresent = 0, classQuantPresent = 0;

    // //considero a primeira classe como minha primeira candidata de moda. então guardo suas infos p dps
    // for( int i = 0; classHolder[i] == classHolder[0]; i++ )
    //     classQuantMost++;
    
    //agora analiso o restante, se tiver no primeiro elemento fora dessa primeira classe,
    //inicializo a análise atual
    classPresent = classHolder[0];
    classQuantPresent++;
    bool drawResult = false;//se der empate recalculo para 1 vizinho a menos
    int drawCounter = 0;//a cada empate eu olho o vetor reduzido de 1, que é o mesmo de diminuir 1 vizinho
  
    do{
        drawResult = false;
        for( int i = 1; i < ( ( (int) classHolder.size() ) - drawCounter ); i++ )
        {
            //se agora estou vendo uma classe diferente da anterior
            if ( classHolder[i] != classPresent )
            {
                //verifico se a classe anterior é a nova moda
                if( classQuantPresent > classQuantMost )
                {
                    //se sim, atualizo a moda
                    #ifdef TESTING_DRAW
                        std::cout << "Sem empates e é a nova moda" << std::endl;
                    #endif
                    classMost = classPresent;
                    classQuantMost = classQuantPresent;
                    drawResult = false;
                    drawCounter = 0;
                } else if( classQuantPresent == classQuantMost )
                {
                    #ifdef TESTING_DRAW
                        std::cout << "Empate em k = " << (( (int) classHolder.size() ) - drawCounter ) << std::endl;
                    #endif
                    drawResult = true;
                    drawCounter++;
                }
                //e começo a analizar essa nova classe
                classPresent = classHolder[i];
                classQuantPresent = 1;
            } else
            {
                //se continuo na mesma classe só aumento o número
                classQuantPresent++;
            }
        }
    } while( drawResult == true );
    return classMost;
}

void Classifier::normalizingZScore( std::vector<Data> &src )
{
    std::vector<float> mean(Classifier::numberAttributes,0.0);
    std::vector<float> std(Classifier::numberAttributes,0.0);
    std::vector<float> aux(Classifier::numberAttributes,0.0);
    for( int i = 0; i < (int) src.size(); i++ )
    {   
        aux = src[i].getAtributes();
        for( int j = 0; j < (int) mean.size(); j++)
        {        
            mean[j] += aux[j];
        }
    }

    for( int i = 0; i < (int) mean.size(); i++)
    {        
        mean[i] /= Classifier::numberSamples;
    }

    for( int i = 0; i < (int) src.size(); i++ )
    {   
        aux = src[i].getAtributes();
        for( int j = 0; j < (int) std.size(); j++)
        {        
            std[j] += ( ( aux[j] - mean[j] ) * ( aux[j] - mean[j] ) );
        }
    }

    for( int i = 0; i < (int) std.size(); i++)
    {        
        std[i] /= ( Classifier::numberSamples - 1 );
        std[i] = std::sqrt ( std[i] );
    }

    for( int i = 0; i < (int) src.size(); i++ )
    {   
        aux = src[i].getAtributes();
        for( int j = 0; j < (int) mean.size(); j++)
        {        
            aux[j] = ( ( aux[j] - mean[j] ) / std[j] );
        }
        src[i].setAtributes( aux );       
    }
}

void Classifier::sortDataByClass( std::vector<Data> &src )
{
    std::vector<Data> aux = src;
    Data auxil = src[0];
    int elementsFold = Classifier::numberSamples / Classifier::kFolds;
    int counterAux = 0;

    #ifdef TESTING_SORT
        char ch;
        Classifier::printDataVector( src );
        ch = getchar();
        std::cout << "------- aux embaixo --------" << std::endl;
        Classifier::printDataVector( aux );
    #endif
    //Primeiro eu ordeno, caso n esteja ordenado, para facilitar a lógica
    for( int i = 0; i < ( (int) src.size() - 1 ); i++ )
    {
        if( src[ i ].getClassNumber() > src[ i+1 ].getClassNumber() )
        {
            auxil = src[i];
            src[i] = src[i+1];
            src[i+1] = auxil;
        }
    }
    #ifdef TESTING_SORT
        Classifier::printDataVector( src );
        ch = getchar();
        std::cout << "------- aux embaixo --------" << std::endl;
        Classifier::printDataVector( aux );
    #endif

    //Agora que já está ordenado, sei que classes repetidas ocorrerão seguidas
    //O primeiro elemento vai manter na mesma posição
    for( int i = 1, w = elementsFold; i < (int) src.size(); i++, w+=elementsFold )
    {
        #ifdef TESTING_SORT
            std::cout<< "i = "<< i << "\nw = " << w << "\n counter = " << counterAux << std::endl;
            ch =getchar();
        #endif

        if( w >= src.size() )
        {
            counterAux++;
            w = counterAux;
        }
        aux[w] = src[i];
    }
    

    src = aux;
}

void Classifier::orderDistanceClassVectors( std::vector<int> &classVector, std::vector<float> &distanceVector )
{
    int classAux;
    float distanceAux;
    for( int i = ( distanceVector.size() - 1 ); i > 0 ; i-- )
    {
        if( distanceVector[i-1] > distanceVector[i] )
        {
            classAux = classVector[i-1];
            distanceAux = distanceVector[i-1];
            classVector[i-1] = classVector[i];
            distanceVector[i-1] = distanceVector[i];
            classVector[i] = classAux;
            distanceVector[i] = distanceAux;
        }
    }

}

float Classifier::distanceMeasurer( Data data1, Data data2 )
{
    float result = 0.0, aux;
    std::vector<float> data1Atrib = data1.getAtributes();
    std::vector<float> data2Atrib = data2.getAtributes();

    #ifdef DISTANCE_MEASURING
        // std::cout << "O que tem no data1: " << std::endl;
        // data1.printData();
        std::cout << "O que eu peguei do data1: " << std::endl;
        for( int i = 0; i < (int) data1Atrib.size(); i++ )
            std::cout << data1Atrib[i] << " ";
        std::cout << std::endl;
        // std::cout << "O que tem no data2: " << std::endl;
        // data2.printData();
        std::cout << "O que eu peguei do data2: " << std::endl;
        for( int i = 0; i < (int) data2Atrib.size(); i++ )
            std::cout << data2Atrib[i] << " ";
        std::cout << std::endl;
    #endif

    for( int i = 0; i < Classifier::numberAttributes; i++ )
    {
        aux = data1Atrib[i] - data2Atrib[i];
        result += ( aux*aux );
        #ifdef DISTANCE_MEASURING
            std::cout << "data1[i] - data2[i] = " << aux <<
                "\n result = " << result << std::endl;
        #endif
    }

    result = std::sqrt( result );
    #ifdef DISTANCE_MEASURING
        std::cout <<" result final = "<< result << std::endl;
    #endif
    return result;
}

void Classifier::printAllData( void )
{
    for( int i = 0; i < Classifier::numberSamples; i++ )
    {
        Classifier::dataSampling[ i ].printData();
    }
}

void Classifier::printDataVector( std::vector<Data> src )
{
    for( int i = 0; i < (int) src.size(); i++ )
    {
       src[ i ].printData();
    }
}

void Classifier::printIntVector( std::vector<int> src )
{
    for( int i = 0; i < (int) src.size(); i++ )
    {
       std::cout<< src[ i ] << " ";
    }
    std::cout << std::endl;
}

void Classifier::printFloatVector( std::vector<float> src )
{
    for( int i = 0; i < (int) src.size(); i++ )
    {
       std::cout<< src[ i ] << " ";
    }
    std::cout << std::endl;
}