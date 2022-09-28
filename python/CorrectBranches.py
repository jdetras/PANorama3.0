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

    if not win.Filename:
        return

    if (win.ImageScale <= 2.0): 
        win.canvas.config(cursor="watch")
        img = win.image.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale + 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.LINEAR)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        if (win.PointId):    
            win.canvas.delete(win.PointId)
            img_to_canv_scale = win.ImageScale/win.Scale
            x = win.Point[0]*img_to_canv_scale 
            y = win.Point[1]*img_to_canv_scale 
            if (win.ptvar.get()=='Addition'):
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[0]) 
            else:
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[1]) 
        win.canvas.config(cursor="arrow")

    return

def ZoomOut( ):

    if not win.Filename:
        return

    if (win.ImageScale >= 0.20): 
        win.canvas.config(cursor="watch")
        img = win.image.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale - 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.ANTIALIAS)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        if (win.PointId):    
            win.canvas.delete(win.PointId)
            img_to_canv_scale = win.ImageScale/win.Scale
            x = win.Point[0]*img_to_canv_scale 
            y = win.Point[1]*img_to_canv_scale 
            if (win.ptvar.get()=='Addition'):
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[0]) 
            else:
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[1]) 
                
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
    win.canvas.pack(expand=YES, fill=BOTH)
    win.canvas.imgId = ''
    return

#  Select bullet at a last branch point

def SelectPoint(event):
    if (win.Filename):
        canv   = event.widget
        if not win.PointId:
            canv_to_img_scale  = win.Scale/win.ImageScale 
            x                  = canv.canvasx(event.x)
            y                  = canv.canvasy(event.y)
            win.Point[0]       = x*canv_to_img_scale # image coordinates
            win.Point[1]       = y*canv_to_img_scale
            if (win.ptvar.get()=='Addition'):
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[0])
            else:
                win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[1])
                
#  Move selected point 

def MovePoint(event):
    if (win.Filename and win.PointId):
        canv   = event.widget
        x      = canv.canvasx(event.x)
        y      = canv.canvasy(event.y)
        tag    = canv.find_closest(x, y)
        if (win.PointId == tag[0]):
            if (win.ptvar.get()=='Addition'):
                win.ptmenubutton.config(menu=win.ptmenu,text=win.ptopt[0],bg=win.PointColor[0])
            else:
                win.ptmenubutton.config(menu=win.ptmenu,text=win.ptopt[1],bg=win.PointColor[1])
            canv_to_img_scale     = win.Scale/win.ImageScale 
            x2               = x*canv_to_img_scale # image coordinates
            y2               = y*canv_to_img_scale
            x1               = win.Point[0]
            y1               = win.Point[1]
            Dx, Dy = (x2 - x1), (y2 - y1)
            win.Point[0] = win.Point[0] + Dx
            win.Point[1] = win.Point[1] + Dy
            img_to_canv_scale  = win.ImageScale/win.Scale 
            Dx, Dy = Dx*img_to_canv_scale, Dy*img_to_canv_scale
            event.widget.move(win.PointId, Dx, Dy)

#  Display panicle image on scrollable canvas

def ViewImage(filename):

    win.image          = Image.open(filename)
    Ws, Hs             = win.image.size
    win.canvas.panicle = PhotoImage(image=win.image)
    win.canvas.imgId   = win.canvas.create_image(0, 0, image=win.canvas.panicle, anchor=NW) 
    win.canvas.config(scrollregion=(0,0,2*Ws,2*Hs))

    return

# Save point 

def CorrectBranch():

    names     = win.Filename.split('/')
    index     = range(0,len(names)-2)
    directory = ''
    for i in index: 
        directory = directory + names[i] + '/'
    if (directory==''):
        directory='./'

    skelname = names[len(names)-1]
    names    = skelname.split('_')
    index    = range(0,len(names)-2)
    basename = ''
    for i in index:
        basename = basename + names[i] + '_'

    basename = basename + names[len(names)-2] 

    if (win.ptvar.get()=='Deletion'):
        command = 'iftCorrectBranches ' + directory + ' ' + basename + ' ' + str(round(win.Point[0])) + ' ' + str(round(win.Point[1])) + ' ' + '-1'
    else:
        command = 'iftCorrectBranches ' + directory + ' ' + basename + ' ' + str(round(win.Point[0])) + ' ' + str(round(win.Point[1])) + ' ' + '+1'

    win.canvas.config(cursor="watch")
    
    os.system(command);
    win.Point         = [0, 0]
    win.canvas.delete(win.PointId)
    win.PointId       = ''
    ViewImage(win.Filename)
    win.canvas.config(cursor="arrow")

    return

# Exit the program

def Quit():

    sys.exit()

# Reset main variables and objects in canvas

def Reset():
    win.Point         = [0, 0]
    win.PointId       = ''
    win.Filename      = ''
    win.ImageScale    = 1.0
    win.Scale         = 1.0
    win.canvas.delete(ALL)

    return

# Load Image

def LoadImage():

    ftypes = [ ('Image files' , '.jpg') ]    
    filename = tkFileDialog.askopenfilename(initialdir='./skeletons',filetypes=ftypes, title='Load a skeleton image')
    if filename:
        Reset()
        win.Filename = filename
        filename = filename.split('/')
        win.CurrImage.set(filename[len(filename)-1])
        ViewImage(win.Filename) 

    return

# Manage PointMenu

def ManagePointMenu():
    ptoption = win.ptvar.get()

    if (ptoption=="Addition"): 
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[0])
    else:
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[1])

    if (win.PointId):    
        win.canvas.delete(win.PointId)
        img_to_canv_scale = win.ImageScale/win.Scale
        x = win.Point[0]*img_to_canv_scale 
        y = win.Point[1]*img_to_canv_scale 
        if (win.ptvar.get()=='Addition'):
            win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[0]) 
        else:
            win.PointId = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[1]) 
        
    return
        

# Next Image

def NextImage():
    
    if (not win.Filename):
        if (os.path.isfile('imagefiles.txt') == True):
            file = open('imagefiles.txt','r')
            name = file.readline()
            name = name.split('.')[0]
            win.Filename = 'skeletons/' + name + '_skel.jpg'            
            win.CurrImage.set(name + '_skel.jpg')
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
            Reset()
            win.Filename =  'skeletons/' + name
            win.CurrImage.set(name)
            ViewImage(win.Filename)

# Usage instructions

def Help():
    win.msg = tkMessageBox.showinfo('Instructions','Place a bullet at the last branch point to add or delete a branch from the panicle. The branch will be added when the bullet is cyan and it will be deleted when the bullet is red.')

#
# Main function
#

if __name__ == "__main__":

    win = Tk()
    win.title('PANorama2.0: Correct skeleton branches by adding/deleting points')    
    win.PointColor    = ['cyan','red']
    win.Point         = [0, 0]
    win.PointId       = ''
    win.Filename      = ''
    win.ImageScale    = 1.0
    win.Scale         = 1.0
 
    win.frame1 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame1.pack(fill=X, padx=5, pady=5)
    Button(win.frame1, text='Load Image', command=LoadImage).pack(side=LEFT)
    Button(win.frame1, text='First/Next Image', command=NextImage).pack(side=LEFT)
    win.ptmenubutton = Menubutton(win.frame1,text="Addition",bg="cyan",relief=RAISED)
    win.ptmenu       = Menu(win.ptmenubutton)
    win.ptopt        = ["Addition","Deletion"]
    win.ptvar        = StringVar()
    for key in win.ptopt:
        win.ptmenu.add_radiobutton(label=key,variable=win.ptvar,value=key,command=ManagePointMenu)

    win.ptvar.set("Addition")
    win.ptmenubutton.config(menu=win.ptmenu) 
    win.ptmenubutton.pack(side=LEFT)

    Button(win.frame1, text='Correct Branch', command=CorrectBranch).pack(side=LEFT)

    Button(win.frame1, text='Help',command=Help).pack(side=RIGHT)

    CreateCanvas(win)

    win.frame2 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame2.pack(fill=X, padx=5, pady=5)

    imgpath         = os.path.expanduser('~') + '/PANorama2.0/icons/'
    zoomIn_img      = PhotoImage(file=imgpath+'zoomin.gif')
    zoomOut_img     = PhotoImage(file=imgpath+'zoomout.gif')
    Button(win.frame2, image=zoomIn_img,command=ZoomIn).pack(side=LEFT)
    Button(win.frame2, image=zoomOut_img,command=ZoomOut).pack(side=LEFT)

    win.CurrImage = StringVar()
    Label(win.frame2, textvariable=win.CurrImage, height=1).pack(side=LEFT)
    win.CurrImage.set(" ")

    Button(win.frame2, text='Quit',command=Quit).pack(side=RIGHT)


    win.mainloop()
