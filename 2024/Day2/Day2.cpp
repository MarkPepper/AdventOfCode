#include <iostream>
#include <fstream>
#include <string>
#include <list>

std::list<std::list<int>> read_text_file() {
    std::string filepath = "./Day2Input.txt";
    std::ifstream file(filepath);
    std::string str;
    std::list<std::list<int>> full_parsed_list;

    while (std::getline(file,str)) {
        // str is our string;
        std::list<int> newlist;
        std::string string_left_to_parse = str.substr(0,-1);

        while (string_left_to_parse.find(" ") != std::string::npos) {
            newlist.push_back(std::stoi(string_left_to_parse.substr(0, string_left_to_parse.find(" "))));
            string_left_to_parse = string_left_to_parse.substr(string_left_to_parse.find(" ") + 1, string_left_to_parse.length());
        }
        newlist.push_back(std::stoi(string_left_to_parse));
        full_parsed_list.push_back(newlist);
    }

    return full_parsed_list;
}

bool is_safe_report(std::list<int> report) {
    auto r_front = report.begin();
    int first_elem = *r_front;
    std::advance(r_front,1);
    int second_elem = *r_front;

    if (first_elem == second_elem) {
        return false;
    }

    // Do this so we can just always check for a valid ascending report.
    if (first_elem > second_elem) {
        report.reverse();
    };
    r_front = report.begin();
    first_elem = *r_front;
    std::advance(r_front,1);
    second_elem = *r_front;

    int prev_val = first_elem;
    while (r_front != report.end()) {
        if ((*r_front - prev_val < 1) or (*r_front - prev_val > 3)) {
            return false;
        }
        prev_val = *r_front;
        r_front++;
    }

    return true;
}

bool is_safe_report_with_problem_dampener(std::list<int> report) {
    // Approach:
    /*
        Check if we can do things 'normally' by dropping either the
        first or the second one.

        THEN, we can define which way everything MUST go by the
        first two elements, but allow one bit of lee-way.
    */
   std::list<int> first_removed = report;
   std::list<int> second_removed = report;
   first_removed.erase(first_removed.begin());
    if (second_removed.size() > 1) {
        auto it = second_removed.begin();
        ++it;
        second_removed.erase(it);
    }

    if (is_safe_report(first_removed) or is_safe_report(second_removed)) {
        return true;
    }
    
    // Done initial check. Now do similar to before, but ignoring a single mistake

    auto r_front = report.begin();
    int first_elem = *r_front;
    r_front++;
    int second_elem = *r_front;

    if (first_elem == second_elem) {
        return false;
    }

    // Do this so we can just always check for a valid ascending report.
    if (first_elem > second_elem) {
        report.reverse();
    };

   first_removed = report;
   second_removed = report;
   first_removed.erase(first_removed.begin());
    if (second_removed.size() > 1) {
        auto it = second_removed.begin();
        ++it;
        second_removed.erase(it);
    }

    if (is_safe_report(first_removed) or is_safe_report(second_removed)) {
        return true;
    }

    r_front = report.begin();
    int errors = 0;
    int prev_val = *r_front;
    ++r_front;

    while ((r_front != report.end())) {
        if ((*r_front - prev_val < 1) or (*r_front - prev_val > 3)) {
            errors += 1;
            if (errors > 1) {
                return false;
            }
            // dont change prev_val in this case.
            r_front++;
        } else {
            prev_val = *r_front;
            r_front++;
        }
    }

    return true;
}

int part1() {
    std::list<std::list<int>> report_list = read_text_file();
    int sum = 0;
    for (auto i = report_list.cbegin(); i != report_list.end(); i++) {
        if (is_safe_report(*i)) {
            sum++;
        }
    }
    return sum;
}

int part2_slow() {
    std::list<std::list<int>> report_list = read_text_file();

    int sum = 0;
    for (auto i = report_list.cbegin(); i != report_list.end(); i++) {
        for (int j = 0; j < (*i).size(); j++) {
            std::list<int> temp_report = *i;
            auto it = temp_report.begin();
            std::advance(it, j);
            temp_report.erase(it);
            if (is_safe_report(temp_report)) {
                sum += 1;
                break;
            }
        }
    }

    return sum;
}

int main() {
    int part1_result = part1();
    std::cout << "Part 1: " << part1_result << "\n";

    int part2_result = part2_slow();
    std::cout << "Part 2: " << part2_result << "\n";

    return 0;
}