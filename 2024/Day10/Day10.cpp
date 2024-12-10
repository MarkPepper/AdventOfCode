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
        return hash1 ^ (hash2 << 1); // Example hash combination
    }
};


std::vector<int> find_number_of_posibilities_from_starting_point(
    int x_loc, 
    int y_loc, 
    int height_val, 
    std::unordered_map<int, std::unordered_map<std::pair<int,int>, std::vector<int>, PairHash>>& previously_found_with_starting_point_i,
    std::vector<std::vector<int>> map
) {
    if (height_val == 9) {
        previously_found_with_starting_point_i[9][std::make_pair(x_loc,y_loc)] = {y_loc * static_cast<int>(map[0].size()) + x_loc};
        return {y_loc * static_cast<int>(map[0].size()) + x_loc};
    }

    std::unordered_map<std::pair<int,int>, std::vector<int>, PairHash> of_interest = previously_found_with_starting_point_i[height_val];

    if (of_interest.find(std::make_pair(x_loc, y_loc)) != of_interest.end()) {
        return of_interest[std::make_pair(x_loc, y_loc)];
    } else {
        std::vector<int> total_paths = {};
        for (int dx : {-1,1}) {
            if (((x_loc + dx) < map[0].size()) and ((x_loc + dx) >= 0)) {
                if ((map[y_loc][x_loc+dx]) == height_val + 1) {
                    auto result = find_number_of_posibilities_from_starting_point(
                        x_loc + dx, 
                        y_loc,
                        height_val + 1,
                        previously_found_with_starting_point_i,
                        map
                    );

                    for (auto i : result) {
                        total_paths.push_back(i);
                    }
                }
            }
        }

        for (int dy : {-1,1}) {
            if (((y_loc + dy) < map.size()) and ((y_loc + dy) >= 0)) {
                if ((map[y_loc + dy][x_loc]) == height_val + 1) {
                    auto result = find_number_of_posibilities_from_starting_point(
                        x_loc, 
                        y_loc + dy,
                        height_val + 1,
                        previously_found_with_starting_point_i,
                        map
                    );

                    for (auto i : result) {
                        total_paths.push_back(i);
                    }
                }
            }
        }

        //FOR PART ONE, UNCOMMENT ALL OF THIS, AND RETURN THE UNIQUE_LIST, INTO THE STUFF BELOW,
        // RATHER THAN THE TOTAL_PATHS THING I MAKE.
        // std::unordered_map<int, int> horrible_hack = {};
        // for (auto i : total_paths) {
        //     if (horrible_hack.find(i) != horrible_hack.end()) {
        //         horrible_hack[i] += 1;
        //     } else {
        //         horrible_hack[i] = 1;
        //     }
        // }

        // std::vector<int> unique_list = {};
        // for (auto& [key, val] : horrible_hack) {
        //     unique_list.push_back(key);
        // }

        previously_found_with_starting_point_i[height_val][std::make_pair(x_loc, y_loc)] = total_paths;
        return total_paths;
    }
}

int part1() {
    std::string filepath = "./Day10Input.txt";
    std::ifstream file(filepath);
    std::string str;

    std::vector<std::vector<int>> map;
    int x_size = 0;
    int y_size = 0;
    while (std::getline(file,str)) {
        ++y_size;
        x_size = str.length();
        std::vector<int> map_line;
        for (int i = 0; i < str.length(); ++i) {
            std::string this_num = str.substr(i,1);
            map_line.push_back(std::stoi(this_num));
        }
        map.push_back(map_line);
    }

    for (auto i : map) {
        for (auto j : i) {
            std::cout << j << ",";
        }
        std::cout << "\n";
    }

    std::unordered_map<int, std::unordered_map<std::pair<int,int>, std::vector<int>, PairHash>> previously_found_with_starting_point_i = {};

    for (int i = 0; i < 10; ++i) {
        previously_found_with_starting_point_i[i] = {};
    }

    int sum = 0;
    for (int y_loc = 0; y_loc < y_size; ++y_loc) {
        for (int x_loc = 0; x_loc < x_size; ++x_loc) {
            if (map[y_loc][x_loc] == 0) {
                auto result = find_number_of_posibilities_from_starting_point(
                    x_loc,
                    y_loc,
                    0,
                    previously_found_with_starting_point_i,
                    map
                );

                sum += result.size();
            }
        }
    }



    std::cout << "Part 1: " << sum << std::endl;

}

int main() {
    /*
        NOTE THAT THE CODE FOR PART 1 AND PART 2 ARE THE SAME, APART
        FROM A COMMENT AT THE END OF ONE OF THE FUNCTIONS WHERE YOU WILL 
        WANT TO RETURN A DIFFERENT THING

        ANNOYINGLY, I HAD A BUG FOR AGES IN PART 1, BECAUSE I HAD MISREAD THE QUESTION,
        BUT EVEN MORE ANNOYINGLY, THIS WAS PRECISELY THE CODE REQUIRED FOR PART 2!!
    */
    part1();
    return 0;
}