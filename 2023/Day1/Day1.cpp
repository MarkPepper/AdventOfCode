#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <regex>

void collect_info() {
    std::string filepath = "Day1Input.txt";
    std::ifstream file(filepath);

    std::string str;
    int sum = 0;
    std::unordered_map<std::string, int> string_map = {
        {"one",1},
        {"two",2},
        {"three",3},
        {"four",4},
        {"five",5},
        {"six",6},
        {"seven",7},
        {"eight",8},
        {"nine",9}
    };

    while (std::getline(file,str)) {
        std::regex regex_str("(?=(one|two|three|four|five|six|seven|eight|nine|[1-9]))");
        std::smatch match;
        std::vector<std::string> matches;
        std::sregex_iterator matches_begin(str.begin(), str.end(), regex_str);
        std::sregex_iterator matches_end;

        int first_num = -1;
        int second_num = 0;

        for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it) {
            std::string substring(it, str.cend());
            if (std::regex_search(substring, match, regex_str)) {
                if (string_map.find(match[1].str()) != string_map.end()) {
                    if (first_num == -1) {
                        first_num = string_map[match[1].str()];
                        second_num = string_map[match[1].str()];
                    } else {
                        second_num = string_map[match[1].str()];
                    }
                } else {
                    if (first_num == -1) {
                        first_num = std::stoi(match[1].str());
                        second_num = std::stoi(match[1].str());
                    } else {
                        second_num = std::stoi(match[1].str());
                    }
                }
            }



        }

        if (first_num == -1) {
            continue;
        }

        sum += 10*first_num + second_num;
    }

    std::cout << sum << std::endl;
}

int main() {
    collect_info();

    return 0;
}