#!/usr/bin/python

import os
import sys


#
# Main function
#

if __name__ == "__main__":

    if len(sys.argv) != 1:
        print 'Usage: ExtractMeasures.py '
        sys.exit()

    os.system('ls > directories.txt')
    file1 = open('directories.txt','r')
    dirname = file1.readline()
    while (dirname):
        dirname = dirname.split('\n')[0]
        if ((dirname != 'PanicleMeasures.csv') and
            (dirname != 'ExtraPanicleMeasures.csv') and
            (dirname != 'directories.txt') and 
            (dirname != 'filenames.txt')):
            command = 'ls ' + dirname + '/originals/*.jpg' + ' > filenames.txt'
            os.system(command)
            file2 = open('filenames.txt','r')
            filename = file2.readline()
            while (filename):
                filename = filename.split('/')[-1]
                filename = filename.split('\n')[0]
                command = 'iftPanicleMeasures ' + './' + dirname + ' ' + filename
                print command
                os.system(command)
                filename = file2.readline()
            file2.close()
        dirname = file1.readline()        
    file1.close()
    os.system('rm directories.txt filenames.txt') 
    sys.exit()
