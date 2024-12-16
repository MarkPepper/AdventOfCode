import numpy as np
import itertools
import heapq

directions = {
    0:[1,0],
    1:[0,1],
    2:[-1,0],
    3:[0,-1]
}
class Map:
    def __init__(self,map_str):
        len_y = len(map_str.split('\n'))
        len_x = len(map_str.split('\n')[0])

        self.walls = np.zeros((len_y,len_x))
        map_split = map_str.split('\n')
        for j in range(len_y):
            for i in range(len_x):
                c = map_split[j][i]
                if c == '#':
                    self.walls[i,j] = 1
                elif c == 'S':
                    self.start = (i,j)
                elif c == 'E':
                    self.finish = (i,j)

        self.dir_ind = 0
        self.visited = set()
        self.part_of_best_path = []
        self.scores_dict = {}


    def legal(self, x, y, dirind):
        if (x,y,dirind) in self.visited:
            return False
        
        if self.walls[x,y] == 1:
            return False

        return True
        
    def dijkstra(self):
        queue = [(0,self.start[0], self.start[1], 0)]

        while queue:
            cost, x, y, dir_ind = heapq.heappop(queue)

            # WE REMOVE THIS FOR PART 2
            # WE FIND THE SHORTEST PATH TO EVERYWHERE
            # THEN WE CAN BACKTRACK TO FIND ALL THE BEST PLACES TO SIT :)
            # if (x,y) == self.finish:
            #     return cost

            
            if (x,y,dir_ind) in self.visited:
                continue
            self.visited.add((x,y,dir_ind))
            self.scores_dict[(x,y,dir_ind)] = cost

            # add our potential lowest values to our queue!
            # three different things we can do:
            # - turn left, right, or move forward.
            # I assume there is no point checking behind us! the only case where it might be useful is on the first 
            # step, but we start in the bottom left so no need.

            dx, dy = directions[dir_ind]
            nx, ny = x + dx, y + dy

            if self.legal(nx,ny,(dir_ind + 1) % 4): #ofc no wall problems, just for checking if we've visited it.
                heapq.heappush(queue, (cost + 1, nx, ny, dir_ind))

            if self.legal(x, y, (dir_ind + 1) % 4):
                heapq.heappush(queue, (cost + 1000, x, y, (dir_ind + 1) % 4))

            if self.legal(x, y, (dir_ind - 1) % 4):
                heapq.heappush(queue, (cost + 1000, x, y, (dir_ind - 1) % 4))


        return -1

    def part1(self):
        min_dist = self.dijkstra()

        min_score = 9e9
        for i in range(4):
            min_score = min(self.scores_dict[(*self.finish, i)], min_score)

        print("Part 1: %s" %min_score)

    def part2(self):
        # i checked by hand that there is only one best solution, but we might aswell add everything because I'm so clever...
        min_score = 9e9
        for i in range(4):
            min_score = min(self.scores_dict[(*self.finish, i)], min_score)
        
        queue = []
        on_path = set()
        for i in range(4):
            if self.scores_dict[(*self.finish, i)] == min_score:
                queue.append((min_score, *self.finish, i))
                on_path.add(self.finish)

        while queue:
            score, x, y, dir_ind = queue.pop()
            for d in [-1,1]:
                if (x,y,(dir_ind - d) % 4) in self.scores_dict:
                    if self.scores_dict[(x,y,(dir_ind - d) % 4)] == (score - 1000):
                        queue.append((score - 1000, x, y, (dir_ind - d) % 4))
                        on_path.add((x,y))

            dx,dy = directions[dir_ind]
            nx,ny = x - dx, y - dy
            if (nx,ny,dir_ind) in self.scores_dict:
                if self.scores_dict[(nx,ny,dir_ind)] == score - 1:
                    queue.append((score - 1, nx, ny, dir_ind))
                    on_path.add((nx,ny))

        print("Part 2: %s" %len(on_path))

def part1():
    filepath = "./Day16Input.txt"

    with open(filepath, 'r') as file:
        map_str = file.read()

    map = Map(map_str)

    map.part1()

def part2():
    filepath = "./Day16Input.txt"

    with open(filepath, 'r') as file:
        map_str = file.read()

    map = Map(map_str)

    # must run part 1 to get part 2 answer
    map.part1()
    map.part2()

part2()

