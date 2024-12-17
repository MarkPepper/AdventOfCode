import numpy as np

def combo_to_val(val,registers):
    if val <= 3:
        return val
    
    if val == 4:
        return registers['A']
    if val == 5:
        return registers['B']
    if val == 6:
        return registers['C']
    else:
        return

#opcode 0
def adv(registers, program, ptr):
    combo_operand = program[ptr+1]
    combo_val = combo_to_val(combo_operand, registers)
    numerator = registers['A']

    registers['A'] =  int(np.floor(numerator / (2 ** combo_val)))
    ptr += 2
    return registers, ptr

#opcode 6
def bdv(registers, program, ptr):
    combo_operand = program[ptr+1]
    combo_val = combo_to_val(combo_operand, registers)
    numerator = registers['A']

    registers['B'] =  int(np.floor(numerator / (2 ** combo_val)))
    ptr += 2
    return registers, ptr

#opcode 7
def cdv(registers, program, ptr):
    combo_operand = program[ptr+1]
    combo_val = combo_to_val(combo_operand, registers)
    numerator = registers['A']

    registers['C'] =  int(np.floor(numerator / (2 ** combo_val)))
    ptr += 2
    return registers, ptr

#opcode 1
def bxl(registers,program,ptr):
    literal_operand = program[ptr+1]
    b_val = registers['B']
    xor_val = literal_operand ^ b_val
    registers['B'] = xor_val
    ptr += 2
    return registers, ptr

#opcode 2
def bst(registers,program,ptr):
    val = combo_to_val(program[ptr+1], registers)
    val = val % 8
    registers['B'] = val
    ptr += 2
    return registers, ptr

#opcode 3
def jnz(registers,program,ptr):
    if (registers['A'] == 0):
        ptr += 2
        return registers, ptr
    else:
        ptr = program[ptr+1]
        return registers,ptr
    
#opcdoe 4
def bxc(registers,program,ptr):
    b_val = registers['B']
    c_val = registers['C']
    bitwise_xor = b_val ^ c_val
    registers['B'] = bitwise_xor
    ptr += 2
    return registers,ptr

#opcode 5
def out(registers,program,ptr):
    combo_op = combo_to_val(program[ptr+1], registers)
    combo_op = combo_op % 8
    print("%s," %combo_op, end="")
    ptr += 2
    return registers,ptr

#opcode 5
def out2(registers,program,ptr):
    combo_op = combo_to_val(program[ptr+1], registers)
    combo_op = combo_op % 8
    ptr += 2
    return registers,ptr,combo_op

opcode_to_fun = {
    0:adv,
    1:bxl,
    2:bst,
    3:jnz,
    4:bxc,
    5:out,
    6:bdv,
    7:cdv
}

def part1():
    registers = {}
    filepath = "./Day17Input.txt"
    with open(filepath, 'r') as file:
        for line in file:
            if line == '\n':
                continue
            if 'A' in line:
                registers['A'] = int(line.split(': ')[1])
            if 'B' in line:
                registers['B'] = int(line.split(': ')[1])
            if 'C' in line:
                registers['C'] = int(line.split(': ')[1])
            if 'Program' in line:
                insts = line.split(': ')[1]
                program = [int(i) for i in insts.split(',')]
    ptr = 0
    max_ptr = len(program)
    while (ptr < max_ptr):
        # print("-=-=-=-=-")
        # print(registers)
        # print(ptr)
        registers,ptr = opcode_to_fun[program[ptr]](registers,program,ptr)
    print("\n")

def get_out_on_this_iter(a_val, registers, program, ptr):
    max_ptr = len(program)
    registers['A'] = a_val
    outputs = []
    while (ptr < max_ptr):
        # print("-=-=-=-=-")
        # print(registers)
        # print(ptr)
        if program[ptr] == 5:
            registers,ptr,output = out2(registers,program,ptr)
            return output
        else:
            registers,ptr = opcode_to_fun[program[ptr]](registers,program,ptr)

    return

def find_possible_a_vals_at_round_n(program, n, allowed_vals_from_next):
    ptr = 0
    val_we_want = program[n]
    N = len(program) - n
    possible_a_vals = [(i*8) + j for i in allowed_vals_from_next for j in range(8)]

    new_possible_a_vals = []
    registers = {'A':0,'B':0,'C':0}
    for a_val in possible_a_vals:
        if (get_out_on_this_iter(a_val, registers, program, 0) == val_we_want):
            new_possible_a_vals.append(a_val)

    return new_possible_a_vals


def part2():
    registers = {}
    filepath = "./Day17Input.txt"
    with open(filepath, 'r') as file:
        for line in file:
            if 'Program' in line:
                insts = line.split(': ')[1]
                program = [int(i) for i in insts.split(',')]
    

    max_ptr = len(program)

    ptr = 0
    registers = {'A':0,'B':0,'C':0}

    start_a_val = ((2**3)) ** (len(program) - 1) - 1
    a_val = start_a_val
    found = False
    possible_starting_a_vals = [i for i in range(1,8)]

    for n in range(len(program)-2, -1, -1):
        print("-=-=-=-=")
        print(n)
        print("number of possible starts: %s" %len(possible_starting_a_vals))
        possible_starting_a_vals =  find_possible_a_vals_at_round_n(program, n, possible_starting_a_vals)

    print("Part 2 Possible Answers: %s" %possible_starting_a_vals)
    print("Part 2: %s" %np.min(possible_starting_a_vals))



part2()