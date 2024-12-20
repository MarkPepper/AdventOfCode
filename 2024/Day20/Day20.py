import numpy as np
import heapq

directions = [
    (1,0),
    (0,1),
    (-1,0),
    (0,-1)
]

class Racetrack:
    def __init__(self, filepath):
        with open(filepath, 'r') as file:
            grid_str = file.read()

        self.len_y = len(grid_str.split('\n'))
        self.len_x = len(grid_str.split('\n')[0])
        self.walls = np.zeros((self.len_x,self.len_y))

        self.standard_costs_list = {}

        for i,line in enumerate(grid_str.split('\n')):
            self.len_x = len(line)
            for j,c in enumerate(line):
                if c == '#':
                    self.walls[j,i] = 1
                elif c == 'S':
                    self.start = (j,i)
                elif c == 'E':
                    self.end = (j,i)
        print("Made our racetrack")

    def is_legal(self, x, y, visited, these_walls):
        if (x,y) in visited:
            return False
        
        if these_walls[x,y] == 1:
            return False
        

        return True
    
    def dijkstra(self, these_walls):
        visited = set()
        queue = [(0,self.start[0], self.start[1])]

        while queue:
            cost,x,y = heapq.heappop(queue)

            if (x,y) in visited:
                continue

            if (x,y) == self.end:
                return cost

            visited.add((x,y))

            for d in directions:
                dx,dy = d
                nx,ny = x + dx, y + dy

                if self.is_legal(nx,ny,visited,these_walls):
                    heapq.heappush(queue,(cost + 1, nx, ny))

        return -1
    
    def dijkstra2(self, these_walls):
        visited = set()
        queue = [(0,self.start[0], self.start[1])]

        while queue:
            cost,x,y = heapq.heappop(queue)

            if (x,y) in visited:
                continue

            visited.add((x,y))
            self.standard_costs_list[(x,y)] = cost

            for d in directions:
                dx,dy = d
                nx,ny = x + dx, y + dy

                if self.is_legal(nx,ny,visited,these_walls):
                    heapq.heappush(queue,(cost + 1, nx, ny))

        return

    def allowed_cheat_destinations_and_costs(self, x_start, y_start):
        allowed_dests_and_distances = []
        for i in range(np.max([x_start - 20,0]), np.min([x_start + 21, self.len_x])):
            x_travel = np.abs(x_start - i)

            for j in range(np.max([0,(y_start - (20 - x_travel))]), np.min([self.len_y, (y_start + (20 - x_travel) + 1)])):
                y_travel = np.abs(y_start - j)
                if self.walls[i,j] == 0:
                    allowed_dests_and_distances.append((i,j,int(x_travel + y_travel)))
                    
        return allowed_dests_and_distances

    
    def part1(self):
        standard_cost = self.dijkstra(self.walls)
        print("Standard cost: %s" %standard_cost)
        sum = 0
        for i in range(1,self.len_x-1):
            for j in range(1,self.len_y-1):
                if self.walls[i,j] == 1:
                    new_walls = self.walls.copy()
                    new_walls[i,j] = 0
                    new_cost = self.dijkstra(new_walls)
                    if (new_cost <= standard_cost - 100):
                        sum += 1
        print("Part 1: %s" %sum)

    def part2(self):
        self.dijkstra2(self.walls)
        standard_full_cost = self.standard_costs_list[self.end]
        sum = 0

        for i in range(self.len_x):
            for j in range(self.len_y):
                if self.walls[i,j] == 1:
                    continue
                # find possible cheats
                allowed_cheats = self.allowed_cheat_destinations_and_costs(i,j)
                start_dist = self.standard_costs_list[(i,j)]
                for cheat in allowed_cheats:
                    end_dist = self.standard_costs_list[(cheat[0],cheat[1])]
                    if ((standard_full_cost - end_dist) + (start_dist + cheat[2]) <= standard_full_cost - 100):
                        sum += 1
        print("Part 2: %s" %sum)


racetrack = Racetrack('./Day20Input.txt')
racetrack.part2()