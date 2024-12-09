import numpy as np

filepath = "./Day7Input.txt"

total_sum = 0
with open(filepath, 'r') as file:
    for line in file:
        aim = int(line.split(':')[0])
        inputs = [int(i) for i in line.split(' ')[1:]]
        
        
        for i in range(3 ** (len(inputs) - 1)):
            sum = inputs[0]
            for j in range(1,len(inputs)):
                base_three = int((np.floor(i / (3 ** (j-1))) % 3))
                if (base_three == 0):
                    sum += inputs[j]
                elif (base_three == 1):
                    sum *= inputs[j]
                else:
                    sum = (sum * (10 ** (int(np.floor(np.log10(inputs[j])) + 1)))) + inputs[j]

            if sum == aim:
                total_sum += aim
                break

print(total_sum)

