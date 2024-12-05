#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

std::unordered_map<int,std::vector<int>> get_manual_rules(std::string filepath) {
    std::ifstream file(filepath);
    std::unordered_map<int,std::vector<int>> rules = {};

    std::string str;
    while (std::getline(file,str)) {
        if ((str.find("|") != std::string::npos)) {
            int first_int = std::stoi(str.substr(0, str.find_first_of("|")));
            int second_int = std::stoi(str.substr(str.find_first_of("|") + 1, str.length()));
            if (rules.find(first_int) != rules.end()) {
                rules[first_int].push_back(second_int);
            } else {
                rules[first_int] = {second_int};
            }
        }
    }

    return rules;
}

std::vector<std::unordered_map<int,int>> get_print_orders(std::string filepath) {
    std::ifstream file(filepath);
    std::vector<std::unordered_map<int,int>> print_orders = {};
    std::string str;
    while (std::getline(file,str)) {
        if ((str.find(",") != std::string::npos)) {
            int count = 0;
            std::unordered_map<int,int> this_manual = {};
            while (str != "") {
                this_manual[std::stoi(str.substr(0, str.find_first_of(",") != std::string::npos ? str.find_first_of(",") : str.length()))] = count;
                ++count;
                str = str.substr(str.find_first_of(",") != std::string::npos ? str.find_first_of(",") + 1 : str.length(), str.length());
            }
            print_orders.push_back(this_manual);
        }
    }

    return print_orders;
}

int check_print_order_and_return_midval(std::unordered_map<int,int> print_order, std::unordered_map<int,std::vector<int>> rules) {
    std::unordered_map<int,int> flipped_print_order = {};
    int count = 0;

    // iterate through the pages
    for (const auto& [num, loc] : print_order) {
        // check if this page needs to be before another page
        if (rules.find(num) != rules.end()) {
            for (int num_to_check : rules[num]) {
                // found that we do need to be before another page. if other page
                // is in print order, make sure it is after, otherwise return nothing
                // as this is illegal manual
                if (print_order.find(num_to_check) != print_order.end()) {
                    if (print_order[num_to_check] < loc) {
                        return 0; // the rule has been broken
                    }
                }
            }
        }
        flipped_print_order[loc] = num;
        ++count;
    }

    int num_to_return = (count - 1) / 2;
    return flipped_print_order[num_to_return];
}

int part1() {
    std::string filepath = "Day5Input.txt";
    std::unordered_map<int,std::vector<int>> rules = get_manual_rules(filepath);
    std::vector<std::unordered_map<int,int>> print_orders = get_print_orders(filepath);

    int sum = 0;

    for (const auto& po : print_orders) {
        sum += check_print_order_and_return_midval(po, rules);
    }

    return sum;
}
int main() {
    int part1result = part1();
    std::cout << "Part 1: " << part1result << std::endl;
    
    return 0;
}