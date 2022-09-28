#!/usr/bin/python3

import tkinter
from tkinter import *
import os
import sys


#
# Main function
#

if __name__ == "__main__":

#    if len(sys.argv) != 4:
#        print 'Usage: CreateSkeletons.py <basename> <first> <last>'
#        sys.exit()
#    elif (sys.argv[2] < 1) or (sys.argv[3] < 1) or (sys.argv[2] > sys.argv[3]):
#        print 'Usage: CreateSkeletons <basename> <first> <last>'
#        sys.exit()
        
    if len(sys.argv) != 1:
        print('Usage: CreateSkeletons')
        sys.exit()

#    indices = range(int(sys.argv[2]),int(sys.argv[3])+1)    

#    for i in indices:
#        filename = sys.argv[1]+str(i)
#        command  = 'iftPanicleSkel '+filename
#        print 'Processing '+command
#        os.system(command)

    if (os.path.isdir('skeletons')==False):
        os.system('mkdir skeletons')
        
    file = open('imagefiles.txt','r')
    filename = file.readline()
    while (filename):
        print('Processing ' + filename)
        command = 'iftPanicleSkel ' + filename
        print(command)
        os.system(command)
        filename = file.readline()
        
    file.close()
    sys.exit()
