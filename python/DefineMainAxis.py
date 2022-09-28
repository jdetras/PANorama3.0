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
        ptlist = range(0,3)
        for i in ptlist:
            if win.SelectedPoint[i]:    
                win.canvas.delete(win.PointId[i])
                img_to_canv_scale = win.ImageScale/win.Scale
                x = win.Point[i][0]*img_to_canv_scale 
                y = win.Point[i][1]*img_to_canv_scale 
                win.PointId[i] = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[i]) 

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

        ptlist = range(0,3)
        for i in ptlist:
            if win.SelectedPoint[i]:    
                win.canvas.delete(win.PointId[i])
                img_to_canv_scale = win.ImageScale/win.Scale
                x = win.Point[i][0]*img_to_canv_scale 
                y = win.Point[i][1]*img_to_canv_scale 
                win.PointId[i] = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=win.PointColor[i]) 

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

#  Select point to define extrusion and panicle main axis

def SelectPoint(event):
    if (win.Filename):
        cur    = win.CurrentPoint
        canv   = event.widget
        if not win.SelectedPoint[cur]:
            color = win.PointColor[cur]
            canv_to_img_scale  = win.Scale/win.ImageScale 
            x                  = canv.canvasx(event.x)
            y                  = canv.canvasy(event.y)
            win.Point[cur][0]  = x*canv_to_img_scale # image coordinates
            win.Point[cur][1]  = y*canv_to_img_scale
            win.PointId[cur]   = win.canvas.create_oval(x-4,y-4,x+4,y+4,fill=color)
            win.SelectedPoint[cur] = True

#  Move selected point to define panicle main axis

def MovePoint(event):
    if (win.Filename):
        canv   = event.widget
        x      = canv.canvasx(event.x)
        y      = canv.canvasy(event.y)
        tag    = canv.find_closest(x, y)
        ptlist = range(0,3)
        for i in ptlist:
            if (win.PointId[i]==tag[0]):
                win.CurrentPoint = i
                win.ptvar.set(win.ptopt[win.CurrentPoint])
                win.ptmenubutton.config(menu=win.ptmenu,text=win.ptopt[win.CurrentPoint],bg=win.PointColor[win.CurrentPoint])
                cur    = win.CurrentPoint
                canv_to_img_scale     = win.Scale/win.ImageScale 
                x2               = x*canv_to_img_scale # image coordinates
                y2               = y*canv_to_img_scale
                x1               = win.Point[cur][0]
                y1               = win.Point[cur][1]
                Dx, Dy = (x2 - x1), (y2 - y1)
                win.Point[cur][0] = win.Point[cur][0] + Dx
                win.Point[cur][1] = win.Point[cur][1] + Dy
                img_to_canv_scale  = win.ImageScale/win.Scale 
                Dx, Dy = Dx*img_to_canv_scale, Dy*img_to_canv_scale
                event.widget.move(win.PointId[cur], Dx, Dy)



#  Display panicle image on scrollable canvas

def ViewImage(filename):

    win.image          = Image.open(filename)
    Ws, Hs             = win.image.size
    win.canvas.panicle = PhotoImage(image=win.image)
    win.canvas.imgId   = win.canvas.create_image(0, 0, image=win.canvas.panicle, anchor=NW) 
    win.canvas.config(scrollregion=(0,0,2*Ws,2*Hs))

    return

# Save point 

def SavePoints():

    ptlist = range(0,3)
    for i in ptlist:
        if not win.SelectedPoint[i]:
            tkMessageBox.showwarning('Save Points','First select all points')
            return
    
    basename = win.Filename.split('.')
    print basename[0]
    filename = basename[0] + '_mainaxis.txt'
    file = open(filename,'w')
    ptlist = range(0,3)
    for i in ptlist:
        x = round(win.Point[i][0])
        y = round(win.Point[i][1])
        outputdata = str(x) + ' ' + str(y) + ' '
        file.write(outputdata)

    win.PointsSaved = True
    file.close()

# Exit the program

def Quit():

    if (not win.PointsSaved):
        number_of_selected_points = 0
        ptlist = range(0,3)
        for i in ptlist:
            if (win.SelectedPoint[i]):
                number_of_selected_points = number_of_selected_points + 1
        if (number_of_selected_points == 0):
            sys.exit()
        else:
            if (tkMessageBox.askyesno('Exiting the program','Do you want to save the selected points before exiting the program?')):
                SavePoints()
                if win.PointsSaved:    
                    sys.exit()
            else:
                sys.exit()
    else:
        sys.exit()

# Reset main variables and objects in canvas

def Reset():
    win.Filename = ''
    win.SelectedPoint = [False, False, False]
    win.PointsSaved = False
    win.canvas.delete(ALL)
    win.ImageScale = 1.0
    win.Scale  = 1.0
    win.CurrentPoint = 0
    win.ptvar.set(win.ptopt[win.CurrentPoint])
    win.ptmenubutton.config(menu=win.ptmenu,text=win.ptopt[win.CurrentPoint],bg=win.PointColor[win.CurrentPoint])

    return

# Load Image

def LoadImage():

    ftypes = [ ('Image files' , '.jpg') ]    
    filename = tkFileDialog.askopenfilename(initialdir='./skeletons',filetypes=ftypes, title='Load a skeleton image')
    if filename:
        ptlist = range(0,3)
        for i in ptlist:
            if (not win.PointsSaved and win.SelectedPoint[i]):
                if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected points before loading a new image?'):
                    SavePoints()
        Reset()
        win.Filename = filename
        filename = filename.split('/')
        win.CurrImage.set(filename[len(filename)-1])
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
            ptlist = range(0,3)
            for i in ptlist:
                if (not win.PointsSaved and win.SelectedPoint[i]):
                    if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected points before loading a new image?'):
                        SavePoints()
                        if not win.PointsSaved:    
                            index = index - 1
                            name  = filelist[index]

            Reset()
            win.Filename =  'skeletons/' + name
            win.CurrImage.set(name)
            ViewImage(win.Filename)
        else:
            ptlist = range(0,3)
            for i in ptlist:
                if (not win.PointsSaved and win.SelectedPoint[i]):
                    if tkMessageBox.askyesno('Loading a new image','Do you want to save the selected points before loading a new image?'):
                        SavePoints()

# Manage PointMenu

def ManagePointMenu():
    ptoption = win.ptvar.get()

    if (ptoption=="Initial Extrusion Point"): 
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[0])
        win.CurrentPoint = 0
    elif (ptoption=="Initial Main Axis Point"):
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[1])
        win.CurrentPoint = 1
    elif (ptoption == "Final Main Axis Point"): 
        win.ptmenubutton.config(menu=win.ptmenu,text=ptoption,bg=win.PointColor[2])
        win.CurrentPoint = 2

# Usage instructions

def Help():
    win.msg = tkMessageBox.showinfo('Instructions','Place bullets to indicate the beginning of the extrusion, the beginning of the main axis, and the end of the main axis, by clicking on the left button and dragging the bullet with the left button pressed. Save the selected points before exiting the program.')

#
# Main function
#

if __name__ == "__main__":


#    if not sys.argv[1:]:
#        print('Usage: DefineMainAxis.py basename')
#        sys.exit(1)

#    filename = sys.argv[1]+'-skel.jpg'

    win = Tk()
    win.title('PANorama2.0: Panicle extrusion and principal axis definition')    

    win.SelectedPoint = [False, False, False]
    win.CurrentPoint  = 0
    win.PointColor    = ['red','orange','cyan']
    win.Point         = [[0, 0], [0, 0], [0, 0]]
    win.PointId       = [0, 0, 0]
    win.PointsSaved   = False
    win.Filename      = ''
    win.ImageScale    = 1.0
    win.Scale         = 1.0
 
    win.frame1 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame1.pack(fill=X, padx=5, pady=5)
    Button(win.frame1, text='Load Image', command=LoadImage).pack(side=LEFT)
    Button(win.frame1, text='Save Points', command=SavePoints).pack(side=LEFT)
    Button(win.frame1, text='First/Next Image', command=NextImage).pack(side=LEFT)
    win.ptmenubutton = Menubutton(win.frame1,text="Initial Extrusion Point",bg="red",relief=RAISED)
    win.ptmenu       = Menu(win.ptmenubutton)
    win.ptopt        = ["Initial Extrusion Point","Initial Main Axis Point","Final Main Axis Point"]
    win.ptvar        = StringVar()
    for key in win.ptopt:
        win.ptmenu.add_radiobutton(label=key,variable=win.ptvar,value=key,command=ManagePointMenu)

    win.ptvar.set("Initial Extrusion Point")
    win.ptmenubutton.config(menu=win.ptmenu) 
    win.ptmenubutton.pack(side=LEFT)

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
