import random
import math 
import sys 
 
argvs = sys.argv  
argc = len(argvs)

n = int(argvs[1])
p = 0.3

# random.seed(1)
def print_adj_list(i, li):
    line = str(i) + ": "
    for j in li:
        line = line + str(j) + " "
    print line
for i in range(1,n+1):
    i_range = range(1, n+1)
    i_range.remove(i)

    sampling_num =  int(round(random.gauss((n-i)/1.5, (n-i)*p)))
    if sampling_num > n-1:
        sampling_num = n-1
    elif sampling_num < 0:
        sampling_num = 0
    adj = sorted([x for x in random.sample(i_range, sampling_num) if x > i])

    print_adj_list(i, adj)

