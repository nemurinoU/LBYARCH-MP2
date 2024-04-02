import glob
import random
import os
from tqdm import tqdm

def double():
    return random.uniform(-999.99, 999.99)

filenames = [file for file in glob.glob('input*.txt')]

last_filename = filenames[-1]
counter = int(last_filename .split('.')[0][-1]) + 1

# random.randint(1, 2**30)
with open(f"input{counter}.txt", "w") as f:
    len = int(2**28)
    f.write(f"{len}\n") # vector length
    f.write(f"{2.0}\n") # scalar variable
    
    # vector X
    for i in tqdm(range(len)):
        f.write(f"{double()} ")
    f.write(f"\n")

    # vector Y
    for i in tqdm(range(len)):
        f.write(f"{double()} ")
    f.write(f"\n")
    pass


