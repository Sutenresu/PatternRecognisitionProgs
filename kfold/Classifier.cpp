#include <cmath>
#include <map>

#include "Classifier.hpp"

using namespace std;

void Classifier::convertHeader(std::string text, int& lines, int& columns){
    std::vector<std::string> tokens;
    std::istringstream iss(text);

    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(tokens));

    lines = atoi(tokens[0].c_str());
    columns = atoi(tokens[1].c_str());
}

Data Classifier::convertLineToData(std::string text, int columns){
    Data data;

    std::vector<std::string> tokens;
    std::istringstream iss(text);

    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(tokens));

    int i;
    for(i = 0; i < columns; i++)
        data.numbers.push_back(strtof(tokens[i].c_str(), 0));

    data.dataClass = atoi(tokens[i].c_str());

    return data;
}

Classifier::Classifier(std::string filePath)
{
    std::ifstream dataText;
    dataText.open(filePath.c_str());

    if (dataText.is_open())
    {
        std::string line;

        getline(dataText, line);

        convertHeader(line, lines, columns);

        while (getline(dataText, line))
            dataVector.push_back(convertLineToData(line, columns));

        dataText.close();
    }
    else
        throw std::invalid_argument("impossivel abrir o arquivo.");
}

//kNeighborsAlgorithm(dataVector[0].numbers, 5, 1, dataVector.size() - 1);
void Classifier::kFoldAlgorithm(int kFolds, int kNeighbors, bool normalize)
{
    if(normalize) dataVector = ZScore();

    int groupSize = dataVector.size()/kFolds;

    int times = 0;
    int rightAnswers = 0;
    for(int i = 0; i < kFolds; i++)
    {
        int startTestGroup = i * groupSize;
        int endTestGroup = startTestGroup + groupSize - 1;
        for(int j = startTestGroup; j <= endTestGroup; j++)
        {
            for(int k = 0; k < kFolds; k++)
            {
                int startTrainingGroup = k * groupSize;
                int endTrainigGroup = startTrainingGroup + groupSize - 1;

                for(int l = startTestGroup; l <= endTrainigGroup; l++)
                {
                    if(l > startTestGroup && l < endTestGroup)
                    {
                        if(kNeighborsAlgorithm(dataVector[l].numbers, kNeighbors, startTestGroup, groupSize) == dataVector[l].dataClass)
                            rightAnswers++;
                        times++;
                    }
                }
            }
        }
    }

    cout << "Times:\t" << times << endl;
    cout << "Certas:\t" << rightAnswers << endl;
    cout << "Porcent:\t" << (float)(rightAnswers * 100) / (float)times << endl;
}

int Classifier::kNeighborsAlgorithm(std::vector<float> data, int kNeighbors, int start, int range)
{
    std::vector<std::pair<float, int> > neighbors;

    for(int i = 0; i < dataVector.size(); i++)
    {
        if(i < start || i >= start + range){
            float sum = 0.0;

            for(int j = 0; j < columns; j++)
            {
                sum += pow(dataVector[i].numbers[j] - data[j], 2.0);
            }

            std::pair<float, int> distance(sqrt(sum), dataVector[i].dataClass);
            neighbors.push_back(distance);
        }

    }

    std::sort(neighbors.begin(), neighbors.end());
    std::map<int,int> classMap;
    for(int i = 0; i < kNeighbors; i++)
        classMap[neighbors[i].second]++;

    std::pair<int, int> dataClass(0,0);
    for (std::map<int,int>::iterator it = classMap.begin(); it != classMap.end(); ++it)
    {
        if(dataClass.second < it->second)
        {
            dataClass.first = it->first;
            dataClass.second = it->second;
        }
    }

    return dataClass.first;
}

//Media: somatoria(xn) / quantidade
//Desvio padrao: raiz quadrada(somatoria((xn - media)^2) / quantidade - 1)
//Normalizado: (xn - media) / desvio padrao
std::vector<Data> Classifier::ZScore()
{
    std::vector<float> averageVector(columns, 0.0);
    std::vector<float> standardDeviationVector(columns, 0.0);
    std::vector<Data> normalizedVector(dataVector.size(), Data(columns));

    for(int i = 0; i < dataVector.size(); i++)
        for(int j = 0; j < columns; j++)
            averageVector[j] += dataVector[i].numbers[j];

    for(int i = 0; i < columns; i++)
        averageVector[i] = averageVector[i] / (float)dataVector.size();

    for(int i = 0; i < dataVector.size(); i++)
        for(int j = 0; j < columns; j++)
            standardDeviationVector[j] += pow(dataVector[i].numbers[j] - averageVector[j], 2.0);

    for(int i = 0; i < columns; i++)
        standardDeviationVector[i] = sqrt(standardDeviationVector[i] / (float)dataVector.size() - 1);

    for(int i = 0; i < dataVector.size(); i++)
    {
        for(int j = 0; j < columns; j++)
            normalizedVector[i].numbers[j] = (dataVector[i].numbers[j] - averageVector[j]) / standardDeviationVector[j];
        normalizedVector[i].dataClass = dataVector[i].dataClass;
    }

    return normalizedVector;
}

void Classifier::ShowData(){
    for(int i = 0; i < dataVector.size(); i++)
        dataVector[i].Show();
}
