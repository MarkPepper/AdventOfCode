import numpy as np

# towel designs

def can_make_towel(towel_to_make, available_towels, known_values):
    sum = 0
    if towel_to_make == '':
        return 1, known_values
    if (towel_to_make in known_values):
        return known_values[towel_to_make], known_values
    else:
        for a in available_towels:
            if a == towel_to_make:
                sum += 1
            elif (towel_to_make[:len(a)] == a):
                result, known_values = can_make_towel(towel_to_make[len(a):],available_towels,known_values)
                sum += result
        
        known_values[towel_to_make] = sum
        return sum, known_values

def part1(filepath):
    with open(filepath,'r') as file:
        text = file.read()
        first_part,second_part = text.split('\n\n')

        available_towels = [i.replace(' ','') for i in first_part.split(',')]
        towels_to_make = second_part.split('\n')

    counter = 0
    for t in towels_to_make:
        temp = can_make_towel(t,available_towels)
        if (temp > 0):
            counter += 1

    print("Part 1: %s" %counter)

def part2(filepath):
    with open(filepath,'r') as file:
        text = file.read()
        first_part,second_part = text.split('\n\n')

        available_towels = [i.replace(' ','') for i in first_part.split(',')]
        towels_to_make = second_part.split('\n')
    
    known_values = {}

    counter = 0
    for t in towels_to_make:
        temp,known_values = can_make_towel(t,available_towels,known_values)
        if (temp > 0):
            counter += temp

    print("Part 2: %s" %counter)

part2("./Day19Input.txt")

    