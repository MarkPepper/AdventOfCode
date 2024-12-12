import numpy as np

def s(n,b,remembered_vals):
    if (not remembered_vals.get((n,b)) is None):
        return remembered_vals.get((n,b)), remembered_vals
    
    if (b == 0):
        remembered_vals[(n,b)] = 1
        return 1, remembered_vals
    
    if (b == 1):
        if (np.floor(np.log10(n)) % 2 == 1):
            remembered_vals[(n,b)] = 2
            return 2, remembered_vals
        else:
            remembered_vals[(n,b)] = 1
            return 1, remembered_vals
        
    if (n == 0):
        val, remembered_vals = s(1,b-1,remembered_vals)
        remembered_vals[(n,b)] = val
        return val,remembered_vals
    
    if (np.floor(np.log10(n)) % 2 == 1):
        first_num = n % (10 ** ((np.floor(np.log10(n)) + 1) / 2))
        second_num = np.floor((n / ((10 ** ((np.floor(np.log10(n)) + 1) / 2)))))
        val1,remembered_vals = s(first_num, b-1,remembered_vals)
        val2,remembered_vals = s(second_num, b-1,remembered_vals)
        remembered_vals[(n,b)] = val1 + val2
        return (val1 + val2), remembered_vals
    
    final_cal,remembered_vals = s(2024*n,b-1,remembered_vals)
    remembered_vals[(n,b)] = final_cal
    return final_cal, remembered_vals
    
def deal_with_number(n):
    if (n == 0):
        return [1]
    
    if (np.floor(np.log10(n)) % 2 == 1):
        first_num = n % (10 ** ((np.floor(np.log10(n)) + 1) / 2))
        second_num = np.floor((n / ((10 ** ((np.floor(np.log10(n)) + 1) / 2)))))
        return [first_num, second_num]
    
    return [n * 2024]
        
def part1():
    filepath = "./Day11Input.txt"

    with open(filepath, 'r') as file:
        for line in file:
            entries = [int(i) for i in line.split(' ')]

        for i in range(25):
            print(i)
            new_entries = []
            for n in entries:
                new_entries += deal_with_number(n)

            entries = new_entries

        print("Part 1: %s" %len(entries))

def part2():
    filepath = "./Day11Input.txt"

    with open(filepath, 'r') as file:
        for line in file:
            entries = [int(i) for i in line.split(' ')]

        sum = 0
        remembered_vals = {}
        for n in entries:
            blink_val,remembered_vals = s(n,75,remembered_vals)
            sum += blink_val

    print("Part 2: %s" %sum)
    


part1()
part2()
        