#include <iostream>
#include <vector>

class Data{
public:
    std::vector<float> numbers;
    int dataClass;

    Data();
    Data(int nNumbers);
    Data(std::vector<float> numbers, int dataClass);

    void Show();
};
