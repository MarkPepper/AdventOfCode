#include <iostream>
#include <list>
#include <utility>
#include <fstream>
#include <string>
#include <unordered_map>

std::pair<std::list<int>, std::list<int> > get_puzzle_input() {
    std::list<int> list1;
    std::list<int> list2;

    std::string file_path = "./Day1Input.txt";
    std::ifstream file(file_path);
    std::string str;
    while (std::getline(file,str)) {
        std::string token = str.substr(0, str.find("   "));
        std::string token2 = str.substr(str.find("   "), -1);

        list1.push_back(std::stoi(token));
        list2.push_back(std::stoi(token2));
        std::cout << token << " + " << token2 << "\n";
        std::cout << std::stoi(token) << " + " << std::stoi(token2) << "\n";
    }

    return std::make_pair(list1, list2);
}

int main() {
    std::cout << "Hello World. \n";
    std::pair<std::list<int>, std::list<int> >  listpair = get_puzzle_input();
    std::cout << " WE HAEV NOW GOT WHAT WE WANTED.";
    std::cout << " -=-=-=-=-=-=-=\n";
    std::cout << "-=-=-=-=-=-=-=-=\n";
    std::list<int> list1 = listpair.first;
    std::list<int> list2 = listpair.second;
    list1.sort();
    list2.sort();


    int sum = 0;
    for (int i = 0; i < 1000; i++) {
        std::cout << list1.front() << " + " << list2.front() << "\n";

        sum += std::abs(list1.front() - list2.front());
        list1.pop_front();
        list2.pop_front();
    }

    std::cout << "Result: " << sum << "\n";
    std::cout << "Start of Part 2." << "\n";

    std::unordered_map<int,int> list_2_valcounts = {};
    list1 = listpair.first;
    list2 = listpair.second;
    for (auto i = list2.cbegin(); i != list2.cend(); i++) {
        if (list_2_valcounts.find(*i) != list_2_valcounts.end()) {
            list_2_valcounts[*i] += 1;
        } else {
            list_2_valcounts[*i] = 1;
        }
    }

    int sum2 = 0;

    for (auto i = list1.cbegin(); i != list1.cend(); i++) {
        if (list_2_valcounts.find(*i) != list_2_valcounts.end()) {
            sum2 += (*i) * list_2_valcounts[*i];
        }
    }

    std::cout << "Result for part 2: " << sum2 << std::endl;

    return 0;
}
