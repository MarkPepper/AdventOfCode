#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <cmath>
#include <unordered_map>
#include <utility> // for std::pair

// Custom hash function for std::pair
struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        // Combine the hashes
        return hash1 ^ (hash2 << 1);
    }
};

int part1() {
    std::string filepath = "./Day8Input.txt";
    std::ifstream file(filepath);
    std::string str;

    std::unordered_map<char, std::unordered_map<std::pair<int,int>, int, PairHash>> char_to_locations;
    std::unordered_map<std::pair<int,int>,int, PairHash> antinode_locations;
    int antinode_loc_sum = 0;
    int x_size = 0;
    int y_size = 0;
    int x_loc = 0;
    int y_loc = 0;
    while (std::getline(file, str)) {
        x_loc = 0;
        ++y_size;
        x_size = str.length();
        
        for (char i : str) {
            if (i != '.') {
                if (char_to_locations.find(i) != char_to_locations.end()) {
                    char_to_locations[i][std::make_pair(x_loc, y_loc)] = 1;
                } else {
                    char_to_locations[i] = {};
                    char_to_locations[i][std::make_pair(x_loc, y_loc)] = 1;
                }
            }
            ++x_loc;
        }
        ++y_loc;
    }

    for (auto& [key,val] : char_to_locations) {
        for (auto& [key2,val2] : char_to_locations[key]) {
            for (auto& [key3, val3] : char_to_locations[key]) {
                if ((key2.first == key3.first) and (key2.second == key3.second)) {
                    continue;
                } else { 
                    int antinode_xloc = key2.first + 2*(key3.first - key2.first);
                    int antinode_yloc = key2.second + 2*(key3.second - key2.second);
                    if ((antinode_xloc >= 0) and (antinode_xloc < x_size) and (antinode_yloc >= 0) and (antinode_yloc < y_size)) {
                        if (antinode_locations.find(std::make_pair(antinode_xloc, antinode_yloc)) != antinode_locations.end()) {
                            continue;
                        } else {
                            antinode_locations[std::make_pair(antinode_xloc, antinode_yloc)] = 1;
                            ++antinode_loc_sum;
                        }
                    }
                }
            }
        }
    }

    std::cout << "Part 1: " << antinode_loc_sum << std::endl;
    return antinode_loc_sum;
}

int part2() {
    std::string filepath = "./Day8Input.txt";
    std::ifstream file(filepath);
    std::string str;

    std::unordered_map<char, std::unordered_map<std::pair<int,int>, int, PairHash>> char_to_locations;
    std::unordered_map<std::pair<int,int>,int, PairHash> antinode_locations;
    int antinode_loc_sum = 0;
    int x_size = 0;
    int y_size = 0;
    int x_loc = 0;
    int y_loc = 0;
    while (std::getline(file, str)) {
        x_loc = 0;
        ++y_size;
        x_size = str.length();
        
        for (char i : str) {
            if (i != '.') {
                if (char_to_locations.find(i) != char_to_locations.end()) {
                    char_to_locations[i][std::make_pair(x_loc, y_loc)] = 1;
                } else {
                    char_to_locations[i] = {};
                    char_to_locations[i][std::make_pair(x_loc, y_loc)] = 1;
                }
            }
            ++x_loc;
        }
        ++y_loc;
    }

    for (auto& [key,val] : char_to_locations) {
        for (auto& [key2,val2] : char_to_locations[key]) {
            for (auto& [key3, val3] : char_to_locations[key]) {
                if ((key2.first == key3.first) and (key2.second == key3.second)) {
                    continue;
                } else { 
                    int antinode_xloc = key2.first;
                    int antinode_xdir = (key3.first - key2.first);
                    int antinode_yloc = key2.second;
                    int antinode_ydir = (key3.second - key2.second);
                    while ((antinode_xloc >= 0) and (antinode_xloc < x_size) and (antinode_yloc >= 0) and (antinode_yloc < y_size)) {
                        if (antinode_locations.find(std::make_pair(antinode_xloc, antinode_yloc)) != antinode_locations.end()) {
                            antinode_xloc += antinode_xdir;
                            antinode_yloc += antinode_ydir;
                            continue;
                        } else {                           
                            antinode_locations[std::make_pair(antinode_xloc, antinode_yloc)] = 1;
                            antinode_xloc += antinode_xdir;
                            antinode_yloc += antinode_ydir;                             
                            ++antinode_loc_sum;
                        }
                    }
                }
            }
        }
    }

    std::cout << "Part 2: " << antinode_loc_sum << std::endl;
    return antinode_loc_sum;
}

int main() {
    part1();
    part2();
    return 0;
}