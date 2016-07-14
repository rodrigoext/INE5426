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