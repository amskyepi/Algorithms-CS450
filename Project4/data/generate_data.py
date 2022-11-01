import string
import random
from random import randint

def get_random_string():
    length = randint(4, 10)
    chars = string.ascii_lowercase
    result_str = ''.join(random.choice(chars) for i in range(length))
    return result_str

data = []
count = 0
inputFile = open('large_input.txt', 'w')
for i in range(500):
    col = []
    for j in range(3):
        col.append(get_random_string())
        col.append(randint(1, 100))
        col.append(randint(1, 5000))
    data.append(col)

inputFile.write('1200\n')
for row in data:
    inputFile.write(f'{row[0]};{row[1]};{row[2]}\n')