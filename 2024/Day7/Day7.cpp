#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <cmath>

// def classic_carryover_adder(
//     arr1,
//     arr2
// ):
//     sum = (arr1 + arr2) % 10
//     overload = np.floor((arr1 + arr2) / 10)
//     overload = np.array([0] + list(overload[:-1]))
//     sum = sum + overload

//     all_carried_over = (np.sum(np.floor((sum) / 10)) == 0)

//     while (not all_carried_over):
//         overload = np.floor(sum / 10)
//         overload = np.array([0] + list(overload[:-1]))
//         sum = sum % 10
//         sum = sum + overload
//         all_carried_over = (np.sum(np.floor((sum) / 10)) == 0)

//     return sum

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

    for (int i = 0; i < n; ++i) {
        multiplication = classic_carryover_adder(multiplication,vec1);
    }

    return multiplication;
}

int calc_result(std::vector<int> inputs, std::vector<int> operations) {
    // the length of the operations should be one less than the inputs
    int total = inputs.front();
    inputs.erase(inputs.begin());
    auto operations_pointer = operations.begin();
    for (int i : inputs) {
        if (*operations_pointer == 0) {
            total += i;
        } else {
            total *= i;
        }
        ++operations_pointer;
    }

    return total;
}

std::vector<int> calc_result2(std::vector<std::vector<int>> inputs_as_arrs, std::vector<int> inputs, std::vector<int> operations) {
    // the length of the operations should be one less than the inputs
    std::vector<int> total = inputs_as_arrs[0];
    inputs.erase(inputs.begin());
    inputs_as_arrs.erase(inputs_as_arrs.begin());

    auto operations_pointer = operations.begin();
    for (int i = 0; i < inputs.size(); ++i) {
        if (*operations_pointer == 0) {
            total = classic_carryover_adder(total, inputs_as_arrs[i]);
        } else {
            total = classic_carryover_multiplication(total, inputs[i]);
        }
        ++operations_pointer;
    }

    return total;
}

std::vector<std::vector<int>> get_all_operations(int operations_length) {
    std::vector<std::vector<int>> all_operations;
    for (int i = 0; i < std::pow(2, operations_length); ++i) {
        std::vector<int> vector(operations_length, 0);
        all_operations.push_back(vector);
    }

    for (int j = 0; j < operations_length; ++j) {
        int mini_counter = 0;
        int current_val = 0;
        for (int i = 0; i < std::pow(2, operations_length); ++i) {
            all_operations.at(i).at(j) = current_val;
            ++mini_counter;
            if (mini_counter == std::pow(2,j)) {
                current_val += 1;
                current_val = current_val % 2;
                mini_counter = 0;
            }
        }
    }

    return all_operations;
}

bool check_arrays_are_same(std::vector<int> num1, std::vector<int> num2) {
    for (int i = 0; i < num1.size(); ++i) {
        if (num1[i] != num2[i]) {
            return false;
        }
    }

    return true;
}   

int part1() {
    std::vector<std::vector<int>> all_operations = get_all_operations(20);
    std::string filepath = "./Day7Input.txt";
    std::string str;
    std::ifstream file(filepath);
    std::vector<int> sum(20,0);

    while (std::getline(file,str)) {
        std::cout << "-=-=-=-=-=-" << std::endl;
        int test_val = 0;

        try {
            test_val = std::stoi(str.substr(0, str.find_first_of(':')));
        } catch(std::out_of_range) {

            std::vector<int> test_val_vec = string_to_vector_num(str.substr(0, str.find_first_of(':')));
            std::vector<std::vector<int>> input_ints_as_vectors = {};
            std::vector<int> input_ints = {};

            std::string the_rest = str.substr(str.find_first_of(':') + 1);
            std::regex digit_finder("\\d{1,3}");
            
            std::sregex_iterator matches_begin(the_rest.begin(),the_rest.end(),digit_finder);
            std::sregex_iterator matches_end;

            for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
                std::smatch m = *i;
                input_ints_as_vectors.push_back(string_to_vector_num(m.str()));
                input_ints.push_back(std::stoi(m.str()));
            }

            auto it = all_operations.begin();
            for (int i = 0; i < std::pow(2,input_ints_as_vectors.size()); ++i) {
                std::vector<int> operation_list = *it;
                if (check_arrays_are_same(calc_result2(input_ints_as_vectors, input_ints, operation_list), test_val_vec)) {
                    std::cout << "That was worth the effort. Adding this one: " << std::endl;
                    std::cout << str << std::endl;
                    for (auto i = sum.end(); i != sum.begin(); true) {
                        --i;
                        std::cout << *i;
                    }
                    std::cout << "\n";
                    for (auto i = test_val_vec.end(); i != test_val_vec.begin(); true) {
                        --i;
                        std::cout << *i;
                    }
                    std::cout << "\n";
                    sum = classic_carryover_adder(sum, test_val_vec);
                    for (auto i = sum.end(); i != sum.begin(); true) {
                        --i;
                        std::cout << *i;
                    }   
                    std::cout << "\n";                 
                    break;
                }
                ++it;
            }
            continue;
        }
        
        std::string the_rest = str.substr(str.find_first_of(':') + 1);
        std::regex digit_finder("\\d{1,5}");
        
        std::sregex_iterator matches_begin(the_rest.begin(),the_rest.end(),digit_finder);
        std::sregex_iterator matches_end;

        std::vector<int> input_ints = {};
        for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
            std::smatch m = *i;
            input_ints.push_back(std::stoi(m.str()));
        }

        auto it = all_operations.begin();
        for (int i = 0; i < std::pow(2,input_ints.size()); ++i) {
            std::vector<int> operation_list = *it;
            if (calc_result(input_ints, operation_list) == test_val) {
                std::vector<int> string_test_result = string_to_vector_num(str.substr(0, str.find_first_of(':')));
                std::cout << str << std::endl;
                for (auto i = sum.end(); i != sum.begin(); true) {
                    --i;
                    std::cout << *i;
                }
                std::cout << "\n";
                for (auto i = string_test_result.end(); i != string_test_result.begin(); true) {
                    --i;
                    std::cout << *i;
                }
                std::cout << "\n";             
                sum = classic_carryover_adder(sum, string_test_result);
                for (auto i = sum.end(); i != sum.begin(); true) {
                    --i;
                    std::cout << *i;
                }
                std::cout << "\n";         
                break;
            }
            ++it;
        }
    }

    std::cout << "Part 1: " << std::endl;

    for (auto i = sum.end(); i != sum.begin(); true) {
        --i;
        std::cout << *i;
    }
    std::cout << "\n" << std::endl;

    return 1;
}

int main() {
    part1();
    return 0;
}