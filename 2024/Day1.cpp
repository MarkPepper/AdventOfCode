#include <iostream>
#include <list>
#include <utility>
#include <fstream>
#include <string>

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

    return 0;
}
