import numpy as np
import re
import time

grid_size_x = 101
grid_size_y = 103
grid_size = np.array([101,103])
np.set_printoptions(threshold=np.inf, linewidth=np.inf)

class Robot:
    def __init__(self, pos, velocity):
        self.pos = pos
        self.velocity = velocity

    def move(self):
        self.pos = (self.pos +  self.velocity) % grid_size

    def get_quadrant(self):
        if (self.pos[0] == 50):
            return 0
        if (self.pos[1] == 51):
            return 0
        if self.pos[0] < 50:
            x_part = 1
        else:
            x_part = 2

        if self.pos[1] < 51:
            y_part = 0
        else:
            y_part = 2

        return x_part + y_part
    

def part1():
    filepath = "./Day14Input.txt"
    robots = []
    with open(filepath, 'r') as file:
        for line in file:
            numbers = [int(i) for i in re.findall(r'-?\b\d+\b', line)]
            robots.append(Robot(np.array([numbers[0], numbers[1]]), np.array([numbers[2],numbers[3]])))

    for r in robots:
        for i in range(100):
            r.move()

    quad_count = {1:0,2:0,3:0,4:0,0:0}
    for r in robots:
        quad_count[r.get_quadrant()] += 1

    ans = quad_count[1] * quad_count[2] * quad_count[3] * quad_count[4]
    print("Part 1: %s" %ans)

def part2():
    filepath = "./Day14Input.txt"
    robots = []
    with open(filepath, 'r') as file:
        for line in file:
            numbers = [int(i) for i in re.findall(r'-?\b\d+\b', line)]
            robots.append(Robot(np.array([numbers[0] % 101, numbers[1] % 103]), np.array([numbers[2],numbers[3]])))


    for i in range(100000):
        arr = np.zeros((101,103))

        quad_count = {1:0,2:0,3:0,4:0,0:0}
        for r in robots:
            quad_count[r.get_quadrant()] += 1
            arr[r.pos[0],r.pos[1]] += 1
            r.move()


        output = np.where(arr != 0, '#', ' ')

        # Convert to strings for printing
        if (quad_count[1] + quad_count[2] < 200):
            print("-=-=-=-=-=-=-=-=-")
            print(i)
            print("-=-=-=-=-=-=-=-=-")
            for row in output:
                print(''.join(row))
            time.sleep(0.5)
        # print(arr)

        
        
part1()
part2()