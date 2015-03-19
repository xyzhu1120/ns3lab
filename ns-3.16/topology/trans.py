#!/usr/bin/python
if __name__=="__main__":
    fd = open("fattree.dat",'r')
    fdout = open("fattree-trans",'w')
    appear = [0]*1000
    edges = fd.readlines()
    count = 0
    for edge in edges:
        nodes = edge.split()
        if appear[int(nodes[0])] == 0:
            appear[int(nodes[0])] = 1
            print>>fdout, nodes[0],count
            count += 1
        if appear[int(nodes[1])] == 0:
            appear[int(nodes[1])] = 1
            print>>fdout, nodes[1],count
            count += 1

