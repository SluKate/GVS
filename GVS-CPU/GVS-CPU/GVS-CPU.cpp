#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

float calculateScalarProduct(vector<float> v1, vector<float> v2)
{
    if (v1.size() != v2.size())
    {
        throw exception("vectors sizes are diferent");
    }

    float res = 0.0f;

    chrono::high_resolution_clock::time_point startTime, endTime;
    
    startTime = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < v1.size(); i++)
    {
        res += v1[i] * v2[i];
    }
    endTime = chrono::high_resolution_clock::now();
    chrono::nanoseconds time = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);
    cout << "Calculation time: " << time.count() <<"ns" << endl;

    return res;
}

class FileReader {
private:
    string filename;
public:
    FileReader(string filename) {
        this->filename = filename;
    }
    vector<float> getVectorFromFile() {
        ifstream in(this->filename);
        vector<float> v;
        string line;
        if (in.is_open()) {
            try {
                while (getline(in, line)) {
                    v.push_back(stof(line));
                }
            }
            catch (...) {
                cout << "File has invalid line" << endl;
                in.close();
                return {};
            }
            in.close();
        }
        return v;
    }
};

int main()
{
    try
    {
        FileReader file1 = FileReader("v1.txt");
        FileReader file2 = FileReader("v2.txt");
        vector<float> v1 = file1.getVectorFromFile();
        vector<float> v2 = file2.getVectorFromFile();

        float res = calculateScalarProduct(v1, v2);

        cout << "Result: " << res;
    }
    catch (const exception& e)
    {
        cout << e.what();
    }
}

