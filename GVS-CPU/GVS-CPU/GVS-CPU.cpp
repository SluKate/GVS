#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <stdexcept> 
#include <sstream> 

using namespace std;

class VectorHandler
{
public:
    static float calculateScalarProduct(vector<float> v1, vector<float> v2)
    {
        if (v1.size() != v2.size())
        {
            throw runtime_error("Vectors sizes are different");
        }

        float res = 0.0f;

        for (size_t i = 0; i < v1.size(); i++)
        {
            res += v1[i] * v2[i];
        }

        return res;
    }
};

class FileReader
{
public:
    template <typename T>
    static vector<T> readToVector(string fileName) {
        ifstream in(fileName);
        vector<T> v;
        string line;

        if (in.is_open()) {
            while (getline(in, line)) {
                stringstream ss(line);
                T value;
                if (ss >> value) {
                    v.push_back(value);
                }
                else {
                    throw runtime_error("Invalid data in file: " + line);
                }
            }
            in.close();
        }
        return v;
    }
};

int main()
{
    try {
        vector<float> v1 = FileReader::readToVector<float>("v1.txt");
        vector<float> v2 = FileReader::readToVector<float>("v2.txt");
        

        chrono::high_resolution_clock::time_point startTime, endTime;

        startTime = chrono::high_resolution_clock::now();
        float res = VectorHandler::calculateScalarProduct(v1, v2);
        endTime = chrono::high_resolution_clock::now();

        chrono::nanoseconds time = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);
        cout << "Время выполнения: " << time.count() << " нс" << endl;

        cout << "Result: " << res << endl;
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
