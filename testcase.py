import random
import time
import subprocess
import pandas as pd

def create_data():
    f = open("input.txt","w")
    f.write("%d %d\n" %(0, random.randint(1,20)))

    for x in range(4):
        arrival_time = random.randint(1,20)
        burst_time = random.randint(1,20)
        print(arrival_time,burst_time)
        f.write("%d %d\n" %(arrival_time, burst_time))
        
    f.close()

def run_optimized():
    subprocess.call(["gcc","optimized.c"])
    subprocess.call("./a.exe")

def run_rr():
    subprocess.call(["gcc","assignment.c"])
    subprocess.call("./a.exe")

i = 0

while i < 100:
    create_data()
    run_optimized()
    run_rr() # TQ = 20

    i += 1

with open("optimized_output.txt","a") as file_object:
    file_object.write("ATT MTT AWT MWT")

with open("RR_output.txt","a") as file_object:
    file_object.write("ATT MTT AWT MWT")

headerlist = ['Avg. TurnaroundTime' , 'Max Turnaround Time', 'Avg. Waiting Time', 'Max Waiting Time']

optimized = pd.read_csv('optimized_output.txt', header=None, delimiter=r"\s+")
rr = pd.read_csv('RR_output.txt', header=None, delimiter=r"\s+")
optimized.to_csv('optimized_output.csv', index=None, header=False)
rr.to_csv('RR_output.csv',index=None, header=False)