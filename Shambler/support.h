#pragma once
#include <iostream>

void print_matrix(std::vector<std::vector<int> > matriz) {
    for(int i = 0; i < matriz.size(); ++i) {
        for (int j = 0; j < matriz[0].size(); ++j) {
            std::cout << matriz[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void sum_matrix(std::vector<std::vector<int>> matriz1, std::vector<std::vector<int>> matriz2) {
    std::vector<std::vector<int> > result = matriz1;
    for (int i = 0; i < matriz1.size(); ++i) {
        for (int j = 0; j < matriz1[i].size(); ++i) {
            result[i][j] = 0;
            result[i][j] = matriz1[i][j] + matriz2[i][j];
        }
    }
    print_matrix(result);
}