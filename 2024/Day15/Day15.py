import numpy as np

directions = {
    '>': np.array([1,0]),
    '<': np.array([-1,0]),
    '^': np.array([0,-1]),
    'v': np.array([0,1])
}

class Room:
    def __init__(self, room_input, instructions):
        y_len = 0
        x_len = 0
        for line in room_input.split('\n'):
            y_len += 1
            x_len = len(line)

        self.boxes = np.zeros((y_len, x_len))
        self.walls = np.zeros((y_len, x_len))

        curr_y = 0
        
        for line in room_input.split('\n'):
            for curr_x in range(x_len):
                if line[curr_x] == '@':
                    self.robot_pos = np.array([curr_x, curr_y])
                elif line[curr_x] == '#':
                    self.walls[curr_x, curr_y] = 1
                elif line[curr_x] != '.':
                    self.boxes[curr_x, curr_y] = 1
            curr_y += 1

        self.instructions = instructions
        self.instruction_num = 0
        self.x_len = x_len
        self.y_len = y_len

    def position_is_in_grid(self, position):
        if (position[0] < 0) or (position[1] < 0) or (position[0] >= self.x_len) or (position[1] >= self.y_len):
            return False
        
        return True
    
    def find_nearest_wall(self):
        dpos = directions[self.instructions[self.instruction_num]]
        npos = self.robot_pos + dpos
        while self.position_is_in_grid(npos):

            if self.walls[npos[0],npos[1]] == 1:
                return npos
            npos += dpos

    def boxes_can_be_pushed(self, nearest_wall, dpos):
        npos = self.robot_pos + dpos

        while (not (npos == nearest_wall).all()):
            if self.boxes[npos[0], npos[1]] == 0:
                return True
            npos += dpos
        return False

    def try_push_boxes(self):
        nearest_wall = self.find_nearest_wall()
        dpos = directions[self.instructions[self.instruction_num]]

        if self.boxes_can_be_pushed(nearest_wall, dpos):
            npos = self.robot_pos + dpos
            self.boxes[npos[0], npos[1]] = 0
            npos += dpos

            while (not (npos == nearest_wall).all()):
                if self.boxes[npos[0], npos[1]] == 0:
                    self.boxes[npos[0], npos[1]] = 1
                    break
                npos += dpos
            self.robot_pos += dpos

            
    def perform_instruction(self):
        dpos = directions[self.instructions[self.instruction_num]]
        npos = self.robot_pos + dpos
        if (self.boxes[npos[0], npos[1]] == 0) and (self.walls[npos[0], npos[1]] == 0):
            self.robot_pos += dpos
        else:
            self.try_push_boxes()
        self.instruction_num += 1

    def part1(self):
        for i in range(len(self.instructions)):
            self.perform_instruction()

        sum = 0
        for i in range(self.x_len):
            for j in range(self.y_len):
                sum += self.boxes[i,j] * ((100 * j) + i)

        print("Part 1: %s" %sum)


class Room2:
    def __init__(self, room_input, instructions):
        y_len = 0
        x_len = 0
        for line in room_input.split('\n'):
            y_len += 1
            x_len = len(line)

        self.boxes = np.zeros((x_len*2,y_len))
        self.walls = np.zeros((x_len*2, y_len))
        curr_y = 0
        for line in room_input.split('\n'):
            for curr_x in range(x_len):
                if line[curr_x] == '@':
                    self.robot_pos = np.array([2*curr_x, curr_y])
                elif line[curr_x] == '#':
                    self.walls[2*curr_x, curr_y] = 1
                    self.walls[2*curr_x + 1, curr_y] = 1
                elif line[curr_x] != '.':
                    self.boxes[2*curr_x, curr_y] = 1
                    self.boxes[2*curr_x + 1, curr_y] = -1
            curr_y += 1

        self.instructions = instructions
        self.instruction_num = 0
        self.x_len = x_len*2
        self.y_len = y_len


    def position_is_in_grid(self, position):
        if (position[0] < 0) or (position[1] < 0) or (position[0] >= self.x_len) or (position[1] >= self.y_len):
            return False
        
        return True
    
    def find_nearest_wall(self, start_pos, dpos):
        npos = start_pos + dpos
        while self.position_is_in_grid(npos):

            if self.walls[npos[0],npos[1]] == 1:
                return npos
            npos += dpos

    def boxes_can_be_pushed_horizontally(self, nearest_wall, dpos):
        npos = self.robot_pos + dpos

        while (not (npos == nearest_wall).all()):
            if self.boxes[npos[0], npos[1]] == 0:
                return True
            npos += dpos
        return False   
    
    def push_boxes_horizontally(self, nearest_wall, dpos):
        # find nearest zero
        npos = self.robot_pos + dpos
        count = 1
        while (not (npos == nearest_wall).all()):
            count += 1
            if self.boxes[npos[0], npos[1]] == 0:
                break
            npos += dpos

        positions = self.robot_pos + np.arange(count)[:,None] * dpos
        values = self.boxes[positions[:,0], positions[:,1]]
        new_values = np.concat([np.array([0]), values[:-1]])
        self.boxes[positions[:,0], positions[:,1]] = new_values

    def can_push_boxes_vertically(self, nloc, dpos, boxes_to_push):
        if (self.boxes[nloc[0], nloc[1]] == 0):
            # nothing to push here!
            return True, boxes_to_push
        
        if (self.boxes[nloc[0], nloc[1]] == 1):
            # we're pushing the left bit
            next_nloc_l = nloc + dpos
            next_nloc_r = nloc + np.array([1,0]) + dpos
        else:
            next_nloc_l = nloc + dpos + np.array([-1,0])
            next_nloc_r = nloc + dpos

        if (self.walls[next_nloc_l[0], next_nloc_l[1]] == 1) or (self.walls[next_nloc_r[0], next_nloc_r[1]] == 1):
            return False, boxes_to_push
        
        boxes_to_push.append(nloc)

        if (self.boxes[nloc[0], nloc[1]] == 1):
            # we're pushing the left bit
            next_nloc_l = nloc + dpos
            next_nloc_r = nloc + np.array([1,0]) + dpos
            boxes_to_push.append(nloc + np.array([1,0]))
        else:
            next_nloc_l = nloc + dpos + np.array([-1,0])
            next_nloc_r = nloc + dpos
            boxes_to_push.append(nloc + np.array([-1,0]))

        if (self.walls[next_nloc_l[0], next_nloc_l[1]] == 1) or (self.walls[next_nloc_r[0], next_nloc_r[1]] == 1):
            return False, boxes_to_push
        elif (self.boxes[next_nloc_l[0], next_nloc_l[1]] == 0) and (self.boxes[next_nloc_r[0], next_nloc_r[1]] == 0):
            return True, boxes_to_push
        else:
            a1_bool, boxes_to_push = self.can_push_boxes_vertically(next_nloc_l, dpos, boxes_to_push)
            a2_bool, boxes_to_push = self.can_push_boxes_vertically(next_nloc_l + np.array([1,0]), dpos, boxes_to_push)
            return (a1_bool and a2_bool), boxes_to_push


    def push_boxes_vertically(self, dpos, boxes_to_push):
        boxes_to_push = sorted(boxes_to_push, key=lambda x: x[1])
        boxes_to_push = list({tuple(arr): arr for arr in boxes_to_push}.values())
        if dpos[1] == 1:
            boxes_to_push.reverse()

        
        for b in boxes_to_push:
            self.boxes[b[0], b[1] + dpos[1]] = self.boxes[b[0], b[1]]
            self.boxes[b[0], b[1]] = 0


    def perform_instruction(self):

        dpos = directions[self.instructions[self.instruction_num]]
        npos = self.robot_pos + dpos

        # don't bother if we immediatelly run into a wall
        if (self.walls[npos[0],npos[1]] == 1):
            self.instruction_num += 1
            return
        
        # otherwise, we must have a box next to us. Check cases going vertically and horizontally
        if (self.instructions[self.instruction_num] == '>') or (self.instructions[self.instruction_num] == '<'):
            nearest_wall = self.find_nearest_wall(self.robot_pos, dpos)
            if self.boxes_can_be_pushed_horizontally(nearest_wall, dpos):
                self.push_boxes_horizontally(nearest_wall,dpos)
                self.robot_pos += dpos

        else:
            we_can_push, boxes_to_push = self.can_push_boxes_vertically(npos, dpos, [])
            if we_can_push:
                self.push_boxes_vertically(dpos,boxes_to_push)
                self.robot_pos += dpos

        self.instruction_num += 1

    def part2(self):
        for i in range(len(self.instructions)):
            self.perform_instruction()

        sum = 0
        for i in range(self.x_len):
            for j in range(self.y_len):
                if self.boxes[i,j] == 1:
                    sum += ((100 * j) + i)

        print(self.boxes.transpose())
        print("Part 2: %s" %sum)


def part1():
    filepath = "./Day15Input.txt"
    with open(filepath, 'r') as file:
        text = file.read()
        room_input, instructions = text.split('\n\n')
        instructions = instructions.replace('\n','')

    room = Room(room_input, instructions)
    room.part1()

def part2():
    filepath = "./Day15Input.txt"
    with open(filepath, 'r') as file:
        text = file.read()
        room_input, instructions = text.split('\n\n')
        instructions = instructions.replace('\n','')

    room = Room2(room_input, instructions)
    room.part2()    

part1()
part2()


