#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <regex>

void read_input_file() {
    std::string filepath = "./Day3Input.txt";
    std::string str;
    std::ifstream file(filepath);

    while (std::getline(file, str)) {
        std::cout << str << std::endl;
    }
}

int calculate_single_mul(std::string mul_str) {
    std::cout << "calculating single mul" << std::endl;
    std::string token = mul_str.substr(mul_str.find("(") + 1, mul_str.find(","));
    std::string token2 = mul_str.substr(mul_str.find(",") + 1, -1);

    return std::stoi(token) * std::stoi(token2);
}

std::pair<int,bool> scan_string_for_mul_instructions(std::string str, bool starting_do) {
    bool current_do = starting_do;
    std::regex regex_str("((mul\\(([1-9]\\d{0,2}),([1-9]\\d{0,2})\\))|(do\\(\\))|(don't\\(\\)))");
    std::smatch m;
    std::sregex_iterator matches_begin(str.begin(), str.end(), regex_str);
    std::sregex_iterator matches_end;

    int sum = 0;
    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        if (match[5].matched) {
            std::cout << "Matched number 4" << std::endl;
            std::cout << match.str() << std::endl;
            current_do = true;
        } else if (match[6].matched) {
            std::cout << "Matched number 5" << std::endl;
            std::cout << match.str() << std::endl;
            current_do = false;
        } else {
            std::string match_str = match.str();
            std::cout << match_str << std::endl;
            if (current_do) {
                sum += calculate_single_mul(match_str);
            }
        }

    }

    return std::make_pair(sum,current_do);
}
int part2() {
    std::string filepath = "./Day3Input.txt";
    std::string str;
    std::ifstream file(filepath);
    bool current_do = true;
    int sum = 0;
    while (std::getline(file,str)) {
        std::pair<int,bool> line_result = scan_string_for_mul_instructions(str,current_do);
        sum += line_result.first;
        current_do = line_result.second;
    }

    return sum;
}
int main() {
    int part1ans = part2();
    std::cout << "Part 2: " << part1ans << std::endl;

    return 0;
}