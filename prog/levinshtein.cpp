#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm> // Включаем заголовочный файл algorithm для std::min
#include <limits>     // Включаем заголовочный файл limits для std::numeric_limits

int edit_dist_td(const std::string &a, const std::string &b, std::vector<std::vector<int>> &matrix, int infinity, int i, int j) {
    if (matrix[i][j] == infinity) {
        if (i == 0) {
            matrix[i][j] = j;
        } else if (j == 0) {
            matrix[i][j] = i;
        } else {
            int ins = edit_dist_td(a, b, matrix, infinity, i, j-1) + 1;
            int del = edit_dist_td(a, b, matrix, infinity, i-1, j) + 1;
            int sub = edit_dist_td(a, b, matrix, infinity, i-1, j-1) + (a[i-1] != b[j-1]);
            matrix[i][j] = std::min({ins, del, sub}); // Используем std::min
        }
    }
    return matrix[i][j];
}

std::vector<std::vector<int>> edit_dist_bu(const std::string &a, const std::string &b, int len_a, int len_b) {
    std::vector<std::vector<int>> matrix(len_a + 1, std::vector<int>(len_b + 1, 0));
    for (int i = 0; i <= len_a; ++i) {
        matrix[i][0] = i;
    }
    for (int j = 1; j <= len_b; ++j) {
        matrix[0][j] = j;
    }
    for (int i = 1; i <= len_a; ++i) {
        for (int j = 1; j <= len_b; ++j) {
            int c = a[i-1] != b[j-1];
            matrix[i][j] = std::min({
                matrix[i-1][j] + 1,
                matrix[i][j-1] + 1,
                matrix[i-1][j-1] + c
            });
        }
    }
    return matrix;
}

std::pair<std::string, std::string> restore(const std::string &a, const std::string &b, const std::vector<std::vector<int>> &matrix, int len_a, int len_b) {
    std::string str_re1, str_re2;
    int i = len_a, j = len_b;
    while (i != 0 || j != 0) {
        if (i != 0 && matrix[i][j] == matrix[i-1][j] + 1) {
            str_re1 += a[i-1];
            str_re2 += '-';
            --i;
        } else if (j != 0 && matrix[i][j] == matrix[i][j-1] + 1) {
            str_re1 += '-';
            str_re2 += b[j-1];
            --j;
        } else {
            str_re1 += a[i-1];
            str_re2 += b[j-1];
            --i;
            --j;
        }
    }
    std::reverse(str_re1.begin(), str_re1.end());
    std::reverse(str_re2.begin(), str_re2.end());
    return {str_re1, str_re2};
}

std::pair<int, std::pair<std::string, std::string>> edit_dist(const std::string &a, const std::string &b, int len_a, int len_b) {
    int infinity = std::numeric_limits<int>::max(); // Используем std::numeric_limits для получения максимального значения для int
    std::vector<std::vector<int>> matrix(len_a + 1, std::vector<int>(len_b + 1, infinity));
    int edit_1 = edit_dist_td(a, b, matrix, infinity, len_a, len_b);
    auto edit_2 = edit_dist_bu(a, b, len_a, len_b);
    auto solution = restore(a, b, matrix, len_a, len_b);
    if (matrix == edit_2) {
        return {edit_1, solution};
    } else {
        std::cerr << "Неверно" << std::endl;
        std::exit(1);
    }
}

int main() {
    std::string str_1 = "editing"; // Declare and initialize str_1
    std::string str_2 = "distance"; // Declare and initialize str_2
    auto result = edit_dist(str_1, str_2, str_1.length(), str_2.length());
    std::cout << result.first << std::endl;
    std::cout << result.second.first << std::endl;
    std::cout << result.second.second << std::endl;
    return 0;
}