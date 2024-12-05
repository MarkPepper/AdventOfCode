#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <vector>

const std::pair<size_t, size_t> find_wordsearch_size() {
    std::string filepath = "Day4Input.txt";
    std::string str;
    std::ifstream file(filepath);

    size_t y_count = 0;
    while (std::getline(file, str)) {
        y_count += 1;
    }

    size_t x_count = str.length();

    return std::make_pair(x_count, y_count);
}

std::vector<std::vector<char> > fetch_data_array(size_t xsize, size_t ysize) {
    std::string filepath = "Day4Input.txt";
    std::ifstream file(filepath);

    std::vector<std::vector<char> > wordsearch(ysize, std::vector<char>(xsize));
    size_t count = 0;

    std::string str;
    while (std::getline(file, str) && count < ysize) {
        for (size_t i = 0; i < xsize && i < str.length(); ++i) {
            wordsearch[count][i] = str[i];
        }
        ++count;
    }

    return wordsearch;
}

int count_from_given_x(int x_xloc, int x_yloc, int grid_xsize, int grid_ysize, std::vector<std::vector<char> > wordsearch) {
    int sum = 0;
    // check horizontal posibilities
    for (int parity : {-1, 1}) {
        if ((x_xloc + (parity * 3) >= 0) and (x_xloc + (parity * 3) < grid_xsize)) {
            std::string test = "";
            for (int i=0; i<4; i++) {
                test += wordsearch[x_yloc][x_xloc + (parity * i)];
            }
            if (test == "XMAS") {
                sum += 1;
            }
        }
    }

    // check vertical
    for (int parity : {-1, 1}) {
        if ((x_yloc + (parity * 3) >= 0) and (x_yloc + (parity * 3) < grid_ysize)) {
            std::string test = "";
            for (int i=0; i < 4; i++) {
                test += wordsearch[x_yloc + (parity * i)][x_xloc];
            }
            if (test == "XMAS") {
                sum += 1;
            }
        }
    }

    // check major diagnoal
    for (int parity : {-1, 1}) {
        if ((x_xloc + (parity * 3) >= 0) and (x_xloc + (parity * 3) < grid_xsize) and (x_yloc + (parity * 3) >= 0) and (x_yloc + (parity * 3) < grid_ysize)) {
            std::string test = "";
            for (int i=0; i<4; i++) {
                test += wordsearch[x_yloc + (parity * i)][x_xloc + (parity * i)];
            }
            if (test == "XMAS") {
                sum += 1;
            }
        }
    }

    // check minor diagnoal
    for (int parity : {-1, 1}) {
        if ((x_xloc + (parity * 3) >= 0) and (x_xloc + (parity * 3) < grid_xsize) and (x_yloc - (parity * 3) >= 0) and (x_yloc - (parity * 3) < grid_ysize)) {
            std::string test = "";
            for (int i=0; i<4; i++) {
                test += wordsearch[x_yloc - (parity * i)][x_xloc + (parity * i)];
            }
            if (test == "XMAS") {
                sum += 1;
            }
        }
    }

    return sum;
}

int part1() {
    const std::pair<size_t, size_t> gridsize = find_wordsearch_size();
    std::vector<std::vector<char> > wordsearch = fetch_data_array(gridsize.first, gridsize.second);

    int total = 0;
    for (int i = 0; i < gridsize.first; ++i) {
        for (int j = 0; j < gridsize.second; ++j) {
            if (wordsearch[j][i] == 'X') {
                total += count_from_given_x(i,j,gridsize.first,gridsize.second,wordsearch);
            }
        }
    }

    return total;
}

int main() {
    int part1ans = part1();

    std::cout << "Part 1: " << part1ans << std::endl;
    return 0;
}