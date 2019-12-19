#include <tools.hpp>

void fill(std::vector<int>& vec, int n){
    vec.resize(n);
    for(int i = 0; i < n; i++){
        std::cin >> vec[i];
    }
}

void fill(std::vector<std::vector<float> >& vec, int n, int k){
    vec.resize(n);
    for(int i = 0; i < n; i++){
        vec[i].resize(k);
        for(int j = 0; j < k; j++){
            std::cin >> vec[i][j];
        }
    }
}

int findMax(std::vector<float>& vec) {
    float max = vec[0];
    int k = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > max) {
            max = vec[i];
            k = i;
        }
    }

    return k;
}

int findMinPositive(std::vector<float>& vec) {
    float min = vec[findMax(vec)];
    int k = -1;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] <= min && vec[i] > 0) {
            min = vec[i];
            k = i;
            
        }
    }
    return k;
}

void transpose(std::vector< std::vector<float> >& vec, int size) {
    int t;
    for(int i = 0; i < size; ++i)
    {
        for(int j = i; j < size; ++j)
        {
            t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}

std::vector<std::vector<float> > transpose(std::vector<std::vector<float> >& A) {
    size_t rows = A.size();
    if (rows == 0) return {{}};
    size_t cols = A[0].size();
    std::vector<std::vector<float>> r(cols, std::vector<float>(rows));
    for (int i = 0; i < rows; ++ i) {
        for (int j = 0; j < cols; ++ j) {
            r[j][i] = A[i][j];
        }
    }
    return r;
}


void printTask() {
    std::cout << R"(Primary task of LP:
    CX->max
    AX<=B
    
Dual task of LP:
    B'Y -> min
    A'Y >= C'
    (A' - transponed matrix)
                    )"<<std::endl;
}
