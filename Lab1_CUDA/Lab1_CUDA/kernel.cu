#include <iostream>
#include <fstream>
#include <vector>
#include <cuda_runtime.h>

__global__ void dotProductKernel(float* a, float* b, float* result, int n) {
    float tmp = 0.0f;
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Каждый поток считает произведение своих элементов
    if (idx < n) {
        tmp = a[idx] * b[idx];
    }

    // Используем атомарное добавление для точного суммирования
    atomicAdd(result, tmp);
}

float dotProductCUDA(float* a, float* b, int n) {
    float* d_a, * d_b, * d_result;
    float h_result = 0.0f;

    cudaMalloc((void**)&d_a, n * sizeof(float));
    cudaMalloc((void**)&d_b, n * sizeof(float));
    cudaMalloc((void**)&d_result, sizeof(float));

    cudaMemcpy(d_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(float), cudaMemcpyHostToDevice);

    // Инициализация результата
    cudaMemset(d_result, 0, sizeof(float));

    int blockSize = 50;
    int numBlocks = (n + blockSize) / blockSize;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    // Запуск ядра
    dotProductKernel << <numBlocks, blockSize >> > (d_a, d_b, d_result, n); // numBlocks по сути количество ядер, но его менять не стоит в данной реализации количество ядер вычисляется по количеству блоков, поэтому меняйте размер блоков это BlockSize

    cudaMemcpy(&h_result, d_result, sizeof(float), cudaMemcpyDeviceToHost);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_result);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    std::cout << "Время выполнения: " << milliseconds << " мс" << std::endl;
    return h_result;
}

bool readVectorFromFile(const std::string& filename, std::vector<float>& vec) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    float value;
    while (file >> value) {
        vec.push_back(value);
    }
    file.close();
    return true;
}

int main() {
    setlocale(LC_ALL, "");
    std::vector<float> a, b;

    if (!readVectorFromFile("v1.txt", a) || !readVectorFromFile("v2.txt", b)) {
        return -1;
    }

    if (a.size() != b.size()) {
        std::cerr << "Размеры векторов не совпадают!" << std::endl;
        return -1;
    }

    float result = dotProductCUDA(a.data(), b.data(), a.size());
    std::cout << "Скалярное произведение: " << result << std::endl;

    return 0;
}
