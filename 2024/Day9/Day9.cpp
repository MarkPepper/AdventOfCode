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

int part1() {
    std::string filepath = "./Day9ExampleInput.txt";
    std::ifstream file(filepath);
    std::string str;
    std::vector<int> full_string = {};
    int file_id_count = 0;
    int parity_counter = 0;

    while (std::getline(file, str)) {
        for (char i : str) {
            std::string i_str = "";
            i_str.push_back(i);
            for (int j = 0; j < std::stoi(i_str); ++j) {
                if ((parity_counter % 2) == 0) {
                    full_string.push_back(file_id_count);
                } else {
                    full_string.push_back(-1);
                }
            }
            ++parity_counter;
            if ((parity_counter % 2) == 0) {
                ++file_id_count;
            }
        }
    }

    for (int i : full_string) {
        std::cout << i << ",";
    }
    std::cout << "\n";

    int* back_ptr = &full_string.back();
    int* front_ptr = &full_string.front();
    while (*front_ptr != -1) {
        ++front_ptr;
    }
    while ((back_ptr > &full_string.front()) and (back_ptr > front_ptr)) {
        if (*back_ptr == -1) {
            --back_ptr;
            continue;
        } else {
            *front_ptr = *back_ptr;
            *back_ptr = -1;
            --back_ptr;
            ++front_ptr;
            while (*front_ptr != -1) {
                ++front_ptr;
            }
        }
    }

    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;

    for (int i : full_string) {
        std::cout << i << ",";
    }
    std::cout << "\n";

    int count = 0;
    std::vector<int> sum(20,0);

    while (full_string[count] != -1) {
        std::cout << full_string[count];

        std::string this_thing = std::to_string(full_string[count]);

        sum = classic_carryover_adder(sum, classic_carryover_multiplication(string_to_vector_num(this_thing), count));
        ++count;
    }


    std::cout << "\n Part 1: " << std::endl;

    for (auto i = sum.end(); i != sum.begin(); true) {
        --i;
        std::cout << *i;
    }
    std::cout << "\n" << std::endl;
    return 0;
}


int part2() {
    std::string filepath = "./Day9Input.txt";
    std::ifstream file(filepath);
    std::string str;

    std::vector<std::pair<int,int>> block_mem;
    int file_id_count = 0;
    int parity_counter = 0;

    while (std::getline(file, str)) {
        for (char i : str) {
            std::string i_str = "";
            i_str.push_back(i);

            if ((parity_counter % 2) == 0) {
                block_mem.push_back(std::make_pair(file_id_count, std::stoi(i_str)));
            } else {
                block_mem.push_back(std::make_pair(-1, std::stoi(i_str)));
            }

            ++parity_counter;
            if ((parity_counter % 2) == 0) {
                ++file_id_count;
            }
        }
    }



    std::pair<int,int>* back_ptr = &block_mem.back();

    while (back_ptr > &block_mem.front()) {
        std::pair<int,int>* front_ptr = &block_mem.front();
        if ((*back_ptr).first == -1) {
            // NOW SORT OUT THE BACK
            // merge with the one ahead
            auto temp_ptr = back_ptr;
            ++temp_ptr;
            if ((*temp_ptr).first == -1) {
                (*back_ptr).second += (*temp_ptr).second;
                std::vector<std::pair<int,int>>::iterator it = block_mem.begin() + (temp_ptr - &block_mem.front());
                block_mem.erase(it);                        
            }
            // merge with the one behind
            temp_ptr = back_ptr;
            --temp_ptr;
            if ((*temp_ptr).first == -1) {
                (*back_ptr).second += (*temp_ptr).second;
                std::vector<std::pair<int,int>>::iterator it = block_mem.begin() + (temp_ptr - &block_mem.front());
                block_mem.erase(it);
                --back_ptr;                        
            }            
            --back_ptr;
            continue;
        }
        while (front_ptr < back_ptr) {
            if ((*front_ptr).first != -1) {
                ++front_ptr;
                if (front_ptr >= back_ptr) {
                    --back_ptr;
                }
                continue;
            } else {
                if ((*front_ptr).second >= (*back_ptr).second) {
                    std::pair<int,int> temp_back = *back_ptr;
                    std::pair<int,int> temp_front = *front_ptr;
                    *front_ptr = *back_ptr;
                    *back_ptr = std::make_pair(-1, temp_back.second);

                    ++front_ptr;
                    if ((*front_ptr).first == -1) {
                        (*front_ptr).second += temp_front.second - temp_back.second;
                    } else {
                        std::vector<std::pair<int,int>>::iterator it = block_mem.begin() + (front_ptr - &block_mem.front());
                        block_mem.insert(it, std::make_pair(-1, temp_front.second - temp_back.second));
                        ++back_ptr;
                    }

                    // NOW SORT OUT THE BACK
                    // merge with the one ahead
                    auto temp_ptr = back_ptr;
                    ++temp_ptr;
                    if ((*temp_ptr).first == -1) {
                        (*back_ptr).second += (*temp_ptr).second;
                        std::vector<std::pair<int,int>>::iterator it = block_mem.begin() + (temp_ptr - &block_mem.front());
                        block_mem.erase(it);    
                    }
                    // merge with the one behind
                    temp_ptr = back_ptr;
                    --temp_ptr;
                    if ((*temp_ptr).first == -1) {
                        (*back_ptr).second += (*temp_ptr).second;
                        std::vector<std::pair<int,int>>::iterator it = block_mem.begin() + (temp_ptr - &block_mem.front());
                        block_mem.erase(it);
                        --back_ptr;                     
                    }
                    front_ptr = back_ptr;
                    --back_ptr;
                } else {
                    ++front_ptr;
                    if (front_ptr >= back_ptr) {
                        --back_ptr;
                    }
                }
            }
        }
    }


    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-" << std::endl;

    std::vector<int> sum(20,0);
    int count = 0;

    for (auto& [key,val] : block_mem) {
        if (key == -1) {
            count += val;
        } else {
            std::string this_thing = std::to_string(key);
            std::vector<int> key_as_vec = string_to_vector_num(this_thing);
            int multiples = 0;
            for (int i = count; i < count + val; ++i) {
                multiples += i;
            }
            sum = classic_carryover_adder(sum, classic_carryover_multiplication(key_as_vec, multiples));
            count += val;
        }
    }


    std::cout << "\n Part 2: " << std::endl;

    for (auto i = sum.end(); i != sum.begin(); true) {
        --i;
        std::cout << *i;
    }
    std::cout << "\n" << std::endl;
    return 0;
}

int main() {
    // std::vector<int> example = {0,1,2,3,4,5,6,7};
    // int* ptr = &example.front();
    // ++ptr;
    // ++ptr;
    // ++ptr;
    // ++ptr; //looking at 4
    // std::cout << "ptr is: " << *ptr << std::endl;
    // auto copy_ptr = ptr;
    // --copy_ptr;
    // std::vector<int>::iterator it = example.begin() + (copy_ptr - &example.front());
    // example.erase(it);

    // for (auto i : example) {
    //     std::cout << i << ", ";
    // }

    // std::cout << "\n What ptr now looks at: " << *ptr << std::endl;

    // copy_ptr = ptr;
    // ++copy_ptr;
    // it = example.begin() + (copy_ptr - &example.front());
    // example.erase(it);

    // for (auto i : example) {
    //     std::cout << i << ", ";
    // }
    // std::cout << "\n What ptr now looks at: " << *ptr << std::endl;

    part2();
    return 0;
}