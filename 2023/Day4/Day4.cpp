#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <regex>
#include <cmath>

void part1() {
    int sum = 0;
    std::string filepath = "Day4Input.txt";
    std::ifstream file(filepath);

    std::string str;
    while (std::getline(file,str)) {
        std::unordered_map<int,int> winning_numbers = {};
        std::unordered_map<int,int> my_numbers = {};

        std::string winning_numbers_string = str.substr(str.find_first_of(':') + 1, str.find_first_of('|') - str.find_first_of(':') - 1);
        
        std::regex digit_finder("\\d{1,3}");
        std::smatch m;
        std::sregex_iterator matches_begin(winning_numbers_string.begin(), winning_numbers_string.end(), digit_finder);
        std::sregex_iterator matches_end;

        for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
            std::smatch match = *i;
            winning_numbers[std::stoi(match.str())] = 1;
        }

        std::string my_numbers_string = str.substr(str.find_first_of("|") + 1, -1);
        std::sregex_iterator matches_begin2(my_numbers_string.begin(), my_numbers_string.end(), digit_finder);
        int count = 0;
        for (std::sregex_iterator i = matches_begin2; i != matches_end; ++i) {
            std::smatch match = *i;
            if (winning_numbers.find(std::stoi(match.str())) != winning_numbers.end()) {
                ++count;
            }
        }

        if (count > 0) {
            sum += std::pow(2,count-1);
        }
    }
    std::cout << "Answer to part 1: " << sum << std::endl;
}

int get_number_of_matches(std::string str) {
    std::unordered_map<int,int> winning_numbers = {};
    std::unordered_map<int,int> my_numbers = {};

    std::string winning_numbers_string = str.substr(str.find_first_of(':') + 1, str.find_first_of('|') - str.find_first_of(':') - 1);

    std::regex digit_finder("\\d{1,3}");
    std::smatch m;
    std::sregex_iterator matches_begin(winning_numbers_string.begin(), winning_numbers_string.end(), digit_finder);
    std::sregex_iterator matches_end;

    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        winning_numbers[std::stoi(match.str())] = 1;
    }

    std::string my_numbers_string = str.substr(str.find_first_of("|") + 1, -1);

    std::sregex_iterator matches_begin2(my_numbers_string.begin(), my_numbers_string.end(), digit_finder);
    int count = 0;
    for (std::sregex_iterator i = matches_begin2; i != matches_end; ++i) {
        std::smatch match = *i;
        if (winning_numbers.find(std::stoi(match.str())) != winning_numbers.end()) {
            ++count;
        }
    }

    return count;
}

void part2() {
    int sum = 0;
    std::string filepath = "Day4Input.txt";
    std::ifstream file(filepath);

    std::unordered_map<int,int> card_counting;

    for (int i = 1; i <= 208; ++i) {
        card_counting[i] = 1;
    }

    std::string str;
    int line_num = 1;
    while (std::getline(file,str)) {
        int number_of_matches = get_number_of_matches(str);
        for (int j = line_num + 1; (j < line_num + 1 + number_of_matches) and (j <= 208); ++j) {
            card_counting[j] += card_counting[line_num];
        }
        sum += card_counting[line_num];
        ++line_num;
    }

    std::cout << "Answer to part 2: " << sum << std::endl;
}
int main() {
    part1();
    part2();
    return 0;
}