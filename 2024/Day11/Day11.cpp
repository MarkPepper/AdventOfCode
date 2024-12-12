#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <cmath>
#include <unordered_map>
#include <utility> // for std::pair

std::vector<int> string_to_vector_num(std::string n) {
    std::reverse(n.begin(), n.end());
    std::vector<int> vector_num(20,0);
    int count = 0;
    for (char i : n) {
        std::string char_i(1,i);
        
        vector_num[count] = std::stoi(char_i);


        ++count;
    }

    return vector_num;
}

std::vector<int> classic_carryover_adder(std::vector<int> vec1, std::vector<int> vec2) {
    // firstly, let's note that we will always deal with vectors of length 1000. I assume this 
    // will be plenty haha.

    std::vector<int> sum(vec1.size(), 0);
    std::vector<int> init_overload(vec1.size() + 1, 0);
    std::vector<int> overload(vec1.size() + 1, 0);
    for (int i = 0; i < vec1.size(); i++) {
        sum[i] = (vec1[i] + vec2[i]) % 10;
        int overload_thing = std::floor((vec1[i] + vec2[i]) / 10);
        overload[i + 1] = overload_thing;
        vec1[i+1] += overload_thing;
    }

    return sum;
}

std::vector<int> classic_carryover_multiplication(std::vector<int> vec1, int n) {
    std::vector<int> multiplication(vec1.size(), 0);
    int size_to_worry_about = std::floor(std::log10(n));
    for (int i = 0; i < size_to_worry_about + 1; ++i) {
        double part1 = n / std::pow(10,i);
        int max_iter = static_cast<int>(part1) % 10;
        for (int j = 0; j < max_iter; ++j) {
            multiplication = classic_carryover_adder(multiplication,vec1);
        }
        
        vec1.insert(vec1.begin(), 0);
        vec1.pop_back();
    }

    return multiplication;
}

std::vector<int> blink_at_stone(int n) {
    if (n == 0) {
        return {1};
    }
    std::string n_as_str = std::to_string(n);
    if (n_as_str.length() % 2 == 0) {
        int first_int = std::stoi(n_as_str.substr(0, n_as_str.length() / 2));
        int second_int = std::stoi(n_as_str.substr(n_as_str.length()/2, -1));

        return {first_int,second_int};
    }

    return {n * 2024};
}

std::vector<std::vector<int>> blink_at_stone2(std::vector<int> n) {
    bool n_is_zero = true;
    for (int i : n) {
        if (i != 0) {
            n_is_zero = false;
            break;
        }
    }
    if (n_is_zero) {
        n[0] = 1;
        return {n};
    }

    int n_length = 0;
    for (int i = 0; i < 20; ++i) {
        if (n[19 - i] != 0) {
            n_length = 20 - i;
            break;
        }
    }

    if (n_length % 2 == 0) {
        std::string n_as_str = "";
        for (int i = 0; i < (n_length); ++i) {
            std::string digit_str = std::to_string(n[i]);
            char digit_car = digit_str[0];
            n_as_str.push_back(digit_car);
        }

        std::vector<int> first_int = string_to_vector_num(n_as_str.substr(0, n_as_str.length() / 2));
        std::vector<int> second_int = string_to_vector_num(n_as_str.substr(n_as_str.length()/2, -1));

        return {first_int,second_int};
    }


    return {classic_carryover_multiplication(n, 2024)};
}

int part1() {
    std::string filepath = "./Day11ExampleInput.txt";
    std::ifstream file(filepath);
    std::string str;
    std::vector<int> stones;

    while (std::getline(file,str)) {
        std::regex digit_finder("\\d{1,9}");
        
        std::sregex_iterator matches_begin(str.begin(),str.end(),digit_finder);
        std::sregex_iterator matches_end;

        for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
            std::smatch m = *i;
            stones.push_back(std::stoi(m.str()));
        }
    }

    std::vector<std::vector<int>> vec_stones;
    for (int i : stones) {
        vec_stones.push_back(string_to_vector_num(std::to_string(i)));
    }

    for (int blink = 0; blink < 25; ++blink) {
        std::vector<std::vector<int>> new_vec = {};
        for (std::vector<int> stone : vec_stones) {
            std::vector<std::vector<int>> blinked_stones = blink_at_stone2(stone);
            for (std::vector<int> s : blinked_stones) {
                new_vec.push_back(s);
            }
        }

        vec_stones = new_vec;
    }

    std::cout << "Part 1: " << vec_stones.size() << std::endl;
    return vec_stones.size();
}

int main() {
    part1();
    return 0;
}