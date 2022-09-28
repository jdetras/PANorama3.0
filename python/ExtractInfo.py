#!/usr/bin/python3

import os
import sys


#
# Main function
#

if __name__ == "__main__":

    if len(sys.argv) != 1:
        print('Usage: ExtractInfo.py ')
        sys.exit()


    file = open('imagefiles.txt','r')
    filename = file.readline()
    while (filename):
        print('Processing ' + filename)
        command = 'iftPanicleInfo ' + filename
        print(command)
        os.system(command)
        filename = file.readline()
        
    file.close()
 
    sys.exit()
