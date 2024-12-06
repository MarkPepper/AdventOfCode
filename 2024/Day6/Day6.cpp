#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <functional> // For std::hash

// Custom hash function for std::array<int, 2>
namespace std {
    template <>
    struct hash<std::array<int, 2>> {
        std::size_t operator()(const std::array<int, 2>& arr) const {
            return std::hash<int>{}(arr[0]) ^ (std::hash<int>{}(arr[1]) << 1);
        }
    };
}

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

class Guard {
    public:
        void turn_right() {
            switch (orientation) {
                case Direction::UP:
                    orientation = Direction::RIGHT;
                    break;
                case Direction::RIGHT:
                    orientation = Direction::DOWN;
                    break;
                case Direction::DOWN:
                    orientation = Direction::LEFT;
                    break;
                case Direction::LEFT:
                    orientation = Direction::UP;
                    break;
                default:
                    break;
            }
        }

        void move() {
            std::array<int, 2> d_pos = dir_to_dx_and_dy[orientation];
            pos_x += d_pos[0];
            pos_y += d_pos[1];
        }

        void move_back() {
            std::array<int, 2> d_pos = dir_to_dx_and_dy[orientation];
            pos_x -= d_pos[0];
            pos_y -= d_pos[1];
        }

        void add_currect_location_to_visited() {
            std::array<int,2> current_pos = {pos_x, pos_y};
            if (visited_locations.find(current_pos) != visited_locations.end()) {
                visited_locations[current_pos] += 1;
            } else {
                visited_locations[current_pos] = 1;
            }
        }

        void add_currect_location_and_dir_to_visited() {
            std::array<int,2> current_pos = {pos_x, pos_y};
            if (visited_loc_and_dir.find(current_pos) != visited_loc_and_dir.end()) {
                visited_loc_and_dir[current_pos].push_back(orientation);
            } else {
                visited_loc_and_dir[current_pos] = {orientation};
            }
        }        

        bool already_seen_this_situation() {
            std::array<int,2> current_pos = {pos_x, pos_y};
            if (visited_loc_and_dir.find(current_pos) != visited_loc_and_dir.end()) {
                auto it = std::find(visited_loc_and_dir[current_pos].begin(), visited_loc_and_dir[current_pos].end(), orientation);
                if (it != visited_loc_and_dir[current_pos].end()) {
                    return true;
                }
            }
            return false;
        }

        void setup_guard(int x, int y, Direction orientation_) {
            pos_x = x;
            pos_y = y;
            orientation = orientation_;
            add_currect_location_to_visited();
        }

        bool about_to_move_out_of_map(int max_x, int max_y) {
            std::array<int, 2> d_pos = dir_to_dx_and_dy[orientation];
            int newx = pos_x + d_pos[0];
            int newy = pos_y + d_pos[1];

            if ((newx < 0) or (newx >= max_x) or (newy < 0) or (newy >= max_y)) {
                return true;
            }

            return false;
        }

        int pos_x;
        int pos_y;
        Direction orientation;
        std::unordered_map<std::array<int,2>, int> visited_locations;
        std::unordered_map<std::array<int,2>, std::vector<Direction>> visited_loc_and_dir;

    private:
        std::unordered_map<Direction, std::array<int, 2>> dir_to_dx_and_dy = {
            // this looks wrong, but 'up' means going to smaller indices
            // this could have been a very tricky bug to find i think.
            {Direction::UP, {0, -1}}, 
            {Direction::RIGHT, {1, 0}},
            {Direction::DOWN, {0, 1}},
            {Direction::LEFT, {-1, 0}}
        };
};

void part1() {
    std::string filepath = "./Day6Input.txt";
    std::string str;
    std::ifstream file(filepath);

    std::vector<std::vector<char>> map;
    Guard guard;

    int y_count = 0;
    int x_count = 0;
    while (std::getline(file, str)) {
        std::vector<char> line = {};
        x_count = str.length();
        int x_track = 0;
        for (char i : str) {
            if (i == '^') {
                guard.setup_guard(x_track, y_count, Direction::UP);
                line.push_back('.');                
            } else if (i == '>') {
                guard.setup_guard(x_track, y_count, Direction::RIGHT);
                line.push_back('.');                
            } else if (i == 'v') {
                guard.setup_guard(x_track, y_count, Direction::DOWN);
                line.push_back('.');
            } else if (i == '<') {
                guard.setup_guard(x_track, y_count, Direction::LEFT);
                line.push_back('.');
            } else {
                line.push_back(i);
            }

            ++x_track;
        }
        map.push_back(line);
        ++y_count;
    }

    int max_x = x_count;
    int max_y = y_count;

    while (not guard.about_to_move_out_of_map(max_x, max_y)) {
        guard.move();
        if (map[guard.pos_y][guard.pos_x] == '#') {
            guard.move_back();
            guard.turn_right();
        } else {
            guard.add_currect_location_to_visited();
        }


    }

    int keys_count = guard.visited_locations.size();

    std::cout << "Part 1: " << keys_count << std::endl;
}

bool check_if_guard_gets_stuck(Guard guard, std::vector<std::vector<char>> map, std::array<int,2> new_wall_loc, int max_x, int max_y) {
    map[new_wall_loc[1]][new_wall_loc[0]] = '#';

    while (not guard.about_to_move_out_of_map(max_x, max_y)) {
        guard.move();
        if (map[guard.pos_y][guard.pos_x] == '#') {
            guard.move_back();
            guard.turn_right();
        } else {
            if (guard.already_seen_this_situation()) {
                guard.visited_loc_and_dir = {};
                return true;
            }
            guard.add_currect_location_and_dir_to_visited();
        }
    }
    guard.visited_loc_and_dir = {};
    return false;
}
void part2() {
    std::string filepath = "./Day6Input.txt";
    std::string str;
    std::ifstream file(filepath);

    std::vector<std::vector<char>> map;
    Guard guard;

    int y_count = 0;
    int x_count = 0;
    while (std::getline(file, str)) {
        std::vector<char> line = {};
        x_count = str.length();
        int x_track = 0;
        for (char i : str) {
            if (i == '^') {
                guard.setup_guard(x_track, y_count, Direction::UP);
                line.push_back('.');                
            } else if (i == '>') {
                guard.setup_guard(x_track, y_count, Direction::RIGHT);
                line.push_back('.');                
            } else if (i == 'v') {
                guard.setup_guard(x_track, y_count, Direction::DOWN);
                line.push_back('.');
            } else if (i == '<') {
                guard.setup_guard(x_track, y_count, Direction::LEFT);
                line.push_back('.');
            } else {
                line.push_back(i);
            }

            ++x_track;
        }
        map.push_back(line);
        ++y_count;
    }

    Guard save_initial_guard = guard;

    int max_x = x_count;
    int max_y = y_count;

    while (not guard.about_to_move_out_of_map(max_x, max_y)) {
        guard.move();
        if (map[guard.pos_y][guard.pos_x] == '#') {
            guard.move_back();
            guard.turn_right();
            guard.add_currect_location_and_dir_to_visited();
        } else {
            guard.add_currect_location_to_visited();
        }
    }


    int part2count = 0;
    for (auto [key,_] : guard.visited_locations) {
        if (check_if_guard_gets_stuck(save_initial_guard, map, key, max_x, max_y)) {
            ++part2count;
        }
    }

    std::cout << "Part 2: " << part2count << std::endl;
    return;
}

int main() {
    part1();
    part2();
    return 0;
}