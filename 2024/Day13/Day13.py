import numpy as np
import re

def smallest_val(a_move, b_move, prize_loc):
    a_cost = 3
    b_cost = 1

    claw_loc = np.array([0,0])

    #maths problem
    # we wish to find the largest b such that there
    # also exists an a for which 
    # a * amove + b * bmove = prize_loc

    # so b * bmove = prize_loc mod amove
    # and a * amove = prize_loc mod bmove
    solutions = []
    for b in range(int(np.min(np.floor(prize_loc / b_move)) + 1)):
        temp = (prize_loc - b*b_move)
        if ((temp/a_move)[0] == (temp/a_move)[1]) and (np.allclose(((prize_loc - (b*b_move)) % a_move), 0.0)):
            a = int(temp[0]/a_move[0])
            solutions.append(np.array([a,b]))
    
    cost_arr = np.array([3,1])
    costs = [int(np.dot(cost_arr, sol)) for sol in solutions]

    if len(costs) == 0:
        return 0
    else:
        return np.min(costs)

def part1():
    filepath = "./Day13ExampleInput.txt"
    part1 = 0
    with open(filepath, 'r') as file:
        sorted_line = False
        a_move = np.array([0.,0.])
        b_move = np.array([0.,0.])
        prize_loc = np.array([0.,0.])
        for line in file:
            if (line == "\n"):
                part1 += smallest_val(a_move, b_move, prize_loc)
            elif ("A" in line):
                a_move = np.array([int(i) for i in re.findall(r'\d+', line)])
            elif ("B" in line):
                b_move = np.array([int(i) for i in re.findall(r'\d+', line)])
            elif ("Prize" in line):
                prize_loc = np.array([int(i) for i in re.findall(r'\d+', line)])

    # quickly do the last one which we miss!
    part1 += smallest_val(a_move, b_move, prize_loc)
    print("Part 1: %s" %part1)

# golly, we actually have to do some linear algebra!!
# Let me quickly get my pen and paper out.
# it's Ma = C, where M = [[a1,b1],[a2,b2]], a = [a,b] from above, and C is the prize loc.
# so a = M^{-1}C.
def smallest_val2(a_move, b_move, prize_loc):
    a_cost = 3
    b_cost = 1
    cost_arr = np.array([3,1])
    solutions = []

    determinant = a_move[0]*b_move[1] - b_move[0]*a_move[1]
    if (determinant != 0):
        inverse = np.array([[b_move[1], -b_move[0]], [-a_move[1], a_move[0]]])
        sol = np.matmul(inverse, prize_loc)
        if (sol[0] % determinant == 0 and sol[1] % determinant == 0):
            solutions.append(sol / determinant)
    else:
        # determinant is 0 what does that mean?
        # it means the two equations are 'parallel'
        # so we should check if we can find a solution at all
        ratio = b_move[0] / b_move[1]
        if prize_loc[0] == ratio*prize_loc[1]:
            # for goodness sakes, we might have solutions.
            # phew! turns out we never ran into the situation.
            # what a relief xD
            print("Need to do more work.")
        else:
            return 0

    
    costs = [int(np.dot(cost_arr, sol)) for sol in solutions]

    if len(costs) == 0:
        return 0
    else:
        return np.min(costs)

def part2():
    filepath = "./Day13Input.txt"
    part2 = 0
    with open(filepath, 'r') as file:
        sorted_line = False
        a_move = np.array([0.,0.])
        b_move = np.array([0.,0.])
        prize_loc = np.array([0.,0.])
        for line in file:
            if (line == "\n"):
                part2 += smallest_val2(a_move, b_move, prize_loc)
            elif ("A" in line):
                a_move = np.array([int(i) for i in re.findall(r'\d+', line)])
            elif ("B" in line):
                b_move = np.array([int(i) for i in re.findall(r'\d+', line)])
            elif ("Prize" in line):
                prize_loc = np.array([int(i) + 10000000000000 for i in re.findall(r'\d+', line)])

    # quickly do the last one which we miss!
    part2 += smallest_val2(a_move, b_move, prize_loc)
    print("Part 2: %s" %part2)

part1()
part2()