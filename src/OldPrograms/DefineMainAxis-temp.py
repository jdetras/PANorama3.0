#!/usr/bin/python

#
# Interface to define the main axis of panicles
# 

from Tkinter import *
from PIL import Image, ImageTk
from PIL.ImageTk import PhotoImage
import tkMessageBox 
import sys
import os
import tkFileDialog


# Grab the cursor's position

def GrabEvent(event):
    win._y = event.y
    win._x = event.x
    return

# Move the image (scrollbars) using the grabbed coordinates as reference

def DragImage(event):
    if (win._y-event.y < 0): 
        win.canvas.yview("scroll",-1,"units")
    elif (win._y-event.y > 0): 
        win.canvas.yview("scroll",1,"units")
    if (win._x-event.x < 0): 
        win.canvas.xview("scroll",-1,"units")
    elif (win._x-event.x > 0): 
        win.canvas.xview("scroll",1,"units")
    win._x = event.x
    win._y = event.y
    return        

        
        
# Scale image on canvas (zoom in/out) using the PIL image win.image as
# the one with the original size

def ZoomIn():

    if (win.ImageScale <= 2.0): 
        win.canvas.config(cursor="watch")
        img = win.image.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale + 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.LINEAR)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        pointlist = range(0,3)
        for i in pointlist:
            if win.SelectedPoint[i]:    
                win.canvas.delete(win.canvas.Pid[i])
                x = win.canvas.Px[i]*win.ImageScale/win.canvas.Scl 
                y = win.canvas.Py[i]*win.ImageScale/win.canvas.Scl 
                win.canvas.Pid[i] = win.canvas.create_oval(x-3,y-3,x+3,y+3,fill=win.PointColor[i]) 

        win.canvas.config(cursor="arrow")

    return

def ZoomOut( ):

    if (win.ImageScale >= 0.20): 
        win.canvas.config(cursor="watch")
        img = win.image.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale - 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.ANTIALIAS)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        pointlist = range(0,3)
        for i in pointlist:
            if win.SelectedPoint[i]:    
                win.canvas.delete(win.canvas.Pid[i])
                x = win.canvas.Px[i]*win.ImageScale/win.canvas.Scl 
                y = win.canvas.Py[i]*win.ImageScale/win.canvas.Scl 
                win.canvas.Pid[i] = win.canvas.create_oval(x-3,y-3,x+3,y+3,fill=win.PointColor[i]) 

        win.canvas.config(cursor="arrow")

    return

def ShowCoords(event):
    print 'Relative Coords of the Event: ', event.x, event.y  
    print 'Absolute Coords of the Event: ', win.canvas.canvasx(event.x), win.canvas.canvasy(event.y)

    return



# Create canvas

def CreateCanvas(win):

    win.canvas  = Canvas(win,width=800, height=600, borderwidth=0)
    win.vbar = Scrollbar(win, orient='vertical')
    win.hbar = Scrollbar(win, orient='horizontal')
    win.vbar.pack(side=RIGHT, fill=Y)
    win.vbar.config(command=win.canvas.yview)
    win.hbar.pack(side=BOTTOM, fill=X)
    win.hbar.config(command=win.canvas.xview)
    win.canvas.config(yscrollcommand=win.vbar.set)
    win.canvas.config(xscrollcommand=win.hbar.set)
    win.canvas.bind('<ButtonPress-1>',SelectPoint)
    win.canvas.bind('<B1-Motion>',MovePoint)
    win.canvas.bind('<ButtonPress-3>',GrabEvent)
    win.canvas.bind('<B3-Motion>',DragImage)
#    win.canvas.bind('<Motion>',ShowCoords)
#    win.canvas.bind('<ButtonRelease-4>',ZoomOut)
#    win.canvas.bind('<ButtonRelease-5>',ZoomIn)
    win.canvas.pack(expand=YES, fill=BOTH)
    

    return

#  Select point to define panicle main axis

def SelectPoint(event):

    if not win.SelectedPoint[win.CurrentPoint]:

        win.canvas.Px[win.CurrentPoint]  = win.canvas.canvasx(event.x)
        win.canvas.Py[win.CurrentPoint]  = win.canvas.canvasy(event.y)
        win.canvas.Scl = win.ImageScale  
        x              = win.canvas.Px[win.CurrentPoint]
        y              = win.canvas.Py[win.CurrentPoint]
        win.canvas.Pid[win.CurrentPoint] = win.canvas.create_oval(x-3,y-3,x+3,y+3,fill=win.PointColor[win.CurrentPoint])
        win.SelectedPoint[win.CurrentPoint] = True
        win.PointSaved    = False    
    return

#  Move selected point to define panicle main axis

def MovePoint(event):
    x   = win.canvas.canvasx(event.x)
    y   = win.canvas.canvasy(event.y)
    tag = win.canvas.find_closest(x, y)
    pointlist = range(0,3)
    for i in pointlist:
        if (tag == win.canvas.Pid[i]):
            x2 = x*win.canvas.Scl/win.ImageScale
            y2 = y*win.canvas.Scl/win.ImageScale
            x1 = win.canvas.Px[i]
            y1 = win.canvas.Py[i]
            Dx, Dy = (x2 - x1), (y2 - y1)
            Dx, Dy = Dx*win.ImageScale/win.canvas.Scl, Dy*win.ImageScale/win.canvas.Scl
            win.canvas.Px[i] = win.canvas.Px[i] + Dx
            win.canvas.Py[i] = win.canvas.Py[i] + Dy
            event.widget.move(win.canvas.Pid[i], Dx, Dy)

    return


#  Display panicle image on scrollable canvas

def ViewImage(filename):

    win.image          = Image.open(filename)
    Ws, Hs             = win.image.size
    win.canvas.panicle = PhotoImage(image=win.image)
    win.canvas.imgId   = win.canvas.create_image(0, 0, image=win.canvas.panicle, anchor=NW) 
    win.canvas.config(scrollregion=(0,0,2*Ws,2*Hs))

    return

# Save point 

def SavePoint():

    basename = win.Filename.split('.')
    print basename[0]
    filename = basename[0] + '_mainaxis.txt'
    file = open(filename,'w')
    pointlist = range(0,3)
    for i in pointlist:
        if win.SelectedPoint[i]:
            x = round(win.canvas.Px[i]/win.canvas.Scl)
            y = round(win.canvas.Py[i]/win.canvas.Scl)
            outputdata = str(x) + ' ' + str(y) + ' '
            file.write(outputdata)
            win.PointSaved = True

    file.close()

# Exit the program

def Quit():
    if (not win.PointSaved and win.SelectedPoint):
        if tkMessageBox.askyesno('Quitting the program','Do you want to save the selected point before quitting the program?'):
            SavePoint()
        
    sys.exit()

# Reset main variables and objects in canvas

def Reset():
    win.Filename = ''
    win.SelectedPoint = False
    win.PointSaved = False
    win.canvas.delete(ALL)
    win.ImageScale = 1.0

    
    return

# Load Image

def LoadImage():



    ftypes = [ ('Image files' , '.jpg') ]    
    filename = tkFileDialog.askopenfilename(initialdir='./skeletons',filetypes=ftypes, title='Load a skeleton image')
    if filename:
        if (not win.PointSaved and win.SelectedPoint):
            if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected point before loading a new image?'):
                SavePoint()
        Reset()
        win.Filename = filename
        ViewImage(win.Filename) 

    return

# Next Image

def NextImage():
    
    if (not win.Filename):
        if (os.path.isfile('imagefiles.txt') == True):
            file = open('imagefiles.txt','r')
            name = file.readline()
            name = name.split('.')[0]
            win.Filename = 'skeletons/' + name + '_skel.jpg'            
            ViewImage(win.Filename)
            file.close()
        return
            
    if (os.path.isfile('imagefiles.txt') == True):
        tmplist = win.Filename.split('/')
        size    = len(tmplist)
        curname = tmplist[size-1].split('\n')[0]

        file = open('imagefiles.txt','r')
        filename = file.readline()
        filelist = [ ] 
        while (filename):
            filelist = filelist + [filename ]
            filename = file.readline()
        file.close()

        size    = len(filelist)
        index   = 0
        for name in filelist:
            name = name.split('.')[0]
            name = name + '_skel.jpg'
            if (name == curname):
                nextimg  = index+1;
            index = index + 1

        if (nextimg < size):
            name = filelist[nextimg]
            name = name.split('.')[0]
            name = name + '_skel.jpg'
            if (not win.PointSaved and win.SelectedPoint):
                if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected point before loading a new image?'):
                    SavePoint()
            Reset()
            win.Filename =  'skeletons/' + name
            ViewImage(win.Filename)
        else:
            if (not win.PointSaved and win.SelectedPoint):
                if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected point before loading a new image?'):
                    SavePoint()


# Manage PointMenu

def ManagePointMenu():
    ptoption = win.ptvar.get()

    if (ptoption=="Extrusion Point"): 
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[0])
        win.CurrentPoint = 0
    elif (ptoption=="Initial Point"):
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[1])
        win.CurrentPoint = 1
    elif (ptoption == "Final Point"): 
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[2])
        win.CurrentPoint = 2
        

# Usage instructions

def Help():
    win.msg = tkMessageBox.showinfo('Instructions','Place a bullet on the terminal point of the main axis by clicking on the left button and dragging the bullet with the left button pressed. Save the selected point before exiting the program.')

#
# Main function
#

if __name__ == "__main__":


#    if not sys.argv[1:]:
#        print('Usage: DefineMainAxis.py basename')
#        sys.exit(1)

#    filename = sys.argv[1]+'-skel.jpg'

    win = Tk()
    win.title('PANorama: Module for panicle extrusion and principal axis definition')    

    win.CurrentPoint  = 0
    win.PointColor    = ["red","green","blue"]
    win.SelectedPoint = [False, False, False]
    win.PointSaved    = False
    win.Filename      = ''
    win.ImageScale    = 1.0
    
    win.frame1 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame1.pack(fill=X, padx=5, pady=5)
    Button(win.frame1, text='Load Image', command=LoadImage).pack(side=LEFT)
    Button(win.frame1, text='Save Point', command=SavePoint).pack(side=LEFT)
    Button(win.frame1, text='First/Next Image', command=NextImage).pack(side=LEFT)
    win.ptmenubutton = Menubutton(win.frame1,text="Extrusion Point",bg="red",relief=RAISED)
    win.ptmenu       = Menu(win.ptmenubutton)
    win.ptvar        = StringVar()
    win.ptopt        = ["Extrusion Point","Initial Point","Final Point"]
    for key in win.ptopt:
        win.ptmenu.add_radiobutton(label=key,variable=win.ptvar,value=key,command=ManagePointMenu)

    win.ptvar.set("Extrusion Point")
    win.ptmenubutton.config(menu=win.ptmenu) 
    win.ptmenubutton.pack(side=LEFT)

    Button(win.frame1, text='Help',command=Help).pack(side=RIGHT)

    CreateCanvas(win)

    win.frame2 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame2.pack(fill=X, padx=5, pady=5)

    imgpath         = os.path.expanduser('~') + '/ift/demo/PanicleProject/'
    zoomIn_img      = PhotoImage(file=imgpath+'zoomin.gif')
    zoomOut_img     = PhotoImage(file=imgpath+'zoomout.gif')
    Button(win.frame2, image=zoomIn_img,command=ZoomIn).pack(side=LEFT)
    Button(win.frame2, image=zoomOut_img,command=ZoomOut).pack(side=LEFT)
    Button(win.frame2, text='Quit',command=Quit).pack(side=RIGHT)


    win.mainloop()
