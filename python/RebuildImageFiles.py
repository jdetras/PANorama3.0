#!/usr/bin/python3

import os
import sys


#
# Main function
#

if __name__ == "__main__":

    if len(sys.argv) != 1:
        print('Usage: RebuildImageFiles.py ')
        sys.exit()

    os.system('ls -v originals > imagefiles.txt')
    file1  = open('imagefiles.txt','r')
    filename = file1.readline()
    print("Images found in originals/")
    while (filename):
        filename = filename.split('\n')[0]
        print(filename)
        filename = file1.readline()
        
