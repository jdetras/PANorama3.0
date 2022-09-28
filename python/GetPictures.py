#!/usr/bin/python3

#
# Interface to define the main axis of panicles
# 
import tkinter
from tkinter import *
from PIL import Image, ImageTk
from PIL.ImageTk import PhotoImage
from tkinter import messagebox
from tkinter import filedialog
import sys
import os
import datetime

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

# Rotate 90

def Rotate90():

    if win.Filename:
        win.canvas.config(cursor="watch")
        win.canvas.update()
        win.skel   = win.skel.rotate(90)
        win.image  = win.image.rotate(90)
        Ws, Hs     = win.skel.size
        win.canvas.delete(win.canvas.imgId)
        img        = win.skel.copy()
        width, height = img.size
        img_resized   = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.ANTIALIAS)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        win.canvas.config(scrollregion=(0,0,2*Ws,2*Hs))
        win.canvas.config(cursor="arrow")

    return
               
# Scale image on canvas (zoom in/out) using the PIL image win.skel as
# the one with the original size

def ZoomIn():

    if not win.Filename:
        return

    if (win.ImageScale <= 2.0): 
        win.canvas.config(cursor="watch")
        win.canvas.update()
        img = win.skel.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale + 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.BICUBIC)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        win.canvas.config(cursor="arrow")

    return

def ZoomOut( ):

    if not win.Filename:
        return

    if (win.ImageScale >= 0.20): 
        win.canvas.config(cursor="watch")
        win.canvas.update()
        img = win.skel.copy()
        width, height = img.size
        win.ImageScale = win.ImageScale - 0.10
        img_resized = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.ANTIALIAS)
        win.canvas.delete(win.canvas.imgId)
        win.canvas.panicle = PhotoImage(image=img_resized)
        win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
        win.canvas.config(cursor="arrow")

    return


# Save Image

def SaveImage():
    
    str = "_";
    seq = ("exp",win.exp.get(),win.idname.get(),win.idnumber.get(),"rep",win.rep.get(),"pan",win.pan.get(),"date",win.date.get())
    basename = str.join( seq );
    filename = basename+".jpg"

    if win.Filename:
        ftypes   = [('Image files', '.jpg')]            
        FileName = tkinter.filedialog.asksaveasfilename(initialdir='./originals',initialfile=filename, filetypes=ftypes, title='Save image')
        if FileName:
            win.image.save(FileName,"jpeg")
            filelist = FileName.split('/')
            i = len(filelist) 
            filename = filelist[i-1]
            noappend = False
            
            if (os.path.isfile('imagefiles.txt') == True):
                file = open('imagefiles.txt','r')
                tmpname = file.readline()
                filelist = []
                while (tmpname):
                    filelist = filelist + [tmpname.split('\n')[0]]
                    tmpname = file.readline()
                file.close()

                for tmpname in filelist:
                    if (tmpname == filename):
                        noappend = True

            if (noappend == False):
                file = open('imagefiles.txt','a')
                file.write(filename+'\n')
                file.close()
    return

# Take Pictures

def TakePicture():

    win.canvas.config(cursor="watch")
    win.canvas.update()

    if (win.Filename):
        Reset()

    if (os.path.isfile('temp.jpg') == True):
        os.system('rm -f temp.jpg')
    if (os.path.isfile('temp_skel.jpg') == True):
        os.system('rm -f temp_skel.jpg')
    
    os.system('env LANG=C gphoto2  --capture-image-and-download --filename=temp.jpg') 
       
    os.system('iftTempPanicleSkel temp.jpg')   
    win.Filename = 'temp.jpg'    
    win.image    = Image.open(win.Filename)
    win.Skelname = 'temp_skel.jpg' 
    ViewImage(win.Skelname) 

    win.canvas.config(cursor="cross")

    return

# Configure Camera

def ConfigCamera():

    os.system('gphoto2 --config')

    return
    
# Add one to exp. #

def AddOneToExp():
    n = int(win.exp.get())
    win.exp.delete(0,END)
    win.exp.insert(str(n+1),str(n+1))
    return

# Add one to rep. #

def AddOneToRep():
    n = int(win.rep.get())
    win.rep.delete(0,END)
    win.rep.insert(str(n+1),str(n+1))
    return

# Add one to pan. #

def AddOneToPan():
    n = int(win.pan.get())
    win.pan.delete(0,END)
    win.pan.insert(str(n+1),str(n+1))
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
    win.canvas.bind('<ButtonPress-3>',GrabEvent)
    win.canvas.bind('<B3-Motion>',DragImage)
    win.canvas.pack(expand=YES, fill=BOTH)
    
    return

#  Display panicle image on scrollable canvas

def ViewImage(skelname):

    win.skel           = Image.open(skelname)
    Ws, Hs             = win.skel.size
    img                = win.skel.copy()
    width, height      = img.size
    win.ImageScale     = 0.30
    img_resized        = img.resize((int(width*win.ImageScale), int(height*win.ImageScale)), Image.ANTIALIAS)
    win.canvas.panicle = PhotoImage(image=img_resized)
    win.canvas.imgId   = win.canvas.create_image(0,0,image=win.canvas.panicle, anchor=NW)
    win.canvas.config(scrollregion=(0,0,2*Ws,2*Hs))

    return

# Exit the program

def Quit():
    sys.exit()

def Reset():
    win.Filename = ''
    win.Skelname = ''
    win.canvas.delete(ALL)
    win.ImageScale = 1.0

    return

# Manage ID

def ManageID():
    win.id.config(text=win.idvar.get())
 
# Usage instructions

def Help():
    win.msg = tkinter.messagebox.showinfo('Instructions','To do ')

#
# Main function
#

if __name__ == "__main__":

    win = Tk()
    win.title('PANorama2.0: Camera Controller')    

    win.Filename      = ''
    win.ImageScale    = 1.0
    
    win.frame1 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame1.pack(fill=X, padx=1, pady=1)
    Button(win.frame1, text='Configure', command=ConfigCamera).pack(side=LEFT)
    Button(win.frame1, text='Take Picture', command=TakePicture).pack(side=LEFT)
    Button(win.frame1, text='Rotate 90', command=Rotate90).pack(side=LEFT)
    Button(win.frame1, text='Save Image', command=SaveImage).pack(side=LEFT)
    Button(win.frame1, text='Help',command=Help).pack(side=RIGHT)

    win.frame2 = Frame(height=5, bd=1, relief=SUNKEN)
    win.frame2.pack(fill=X, padx=1, pady=1)

    Button(win.frame2,text='Exp. #',command=AddOneToExp).grid(row=0,column=0)
    win.exp = Entry(win.frame2)
    win.exp.grid(row=0,column=1)
    win.exp.insert(0,"") 

    Label(win.frame2,text='ID name').grid(row=0,column=2)
    win.idname = Entry(win.frame2)
    win.idname.grid(row=0,column=3)
    win.idname.insert(0,"") 

    #win.id = Menubutton(win.frame2,text="nsftv",bg="yellow",relief=RAISED)
    #win.idmenu = Menu(win.id)
    #win.idvar  = StringVar()
    #win.idopt  = ["nsftv","nihtv"]
    #for key in win.idopt:
    #    win.idmenu.add_radiobutton(label=key,variable=win.idvar,value=key,command=ManageID)
    #win.idvar.set("nsftv")
    #win.id.config(menu=win.idmenu) 
    #win.id.grid(row=0,column=2)

    Label(win.frame2,text='ID #').grid(row=0,column=4)
    win.idnumber = Entry(win.frame2)
    win.idnumber.grid(row=0,column=5)
    win.idnumber.insert(0,"") 


    Button(win.frame2,text='Rep. #', command=AddOneToRep).grid(row=0,column=6)
    win.rep = Entry(win.frame2)
    win.rep.grid(row=0,column=7)
    win.rep.insert(0,"") 

    Button(win.frame2,text='Pan. #', command=AddOneToPan).grid(row=0,column=8)
    win.pan = Entry(win.frame2)
    win.pan.grid(row=0,column=9)
    win.pan.insert(0,"") 

    now  = datetime.datetime.now()
    Label(win.frame2,text='Date').grid(row=0,column=10)
    win.date = Entry(win.frame2)
    win.date.grid(row=0,column=11)
#    '%Y'.format(now.year)
#    '%m'.format(now.month)
#    '%d'.format(now.day)
#    date = '{0:02d}{1:02d}{2:04d}'.format(now.month,now.day,now.year)
#    win.date.insert(int(date),date)
    win.date.insert(0,"")

    CreateCanvas(win)

    if (os.path.isdir('originals')==False):
        os.system('mkdir originals')

    win.frame3 = Frame(height=100, bd=1, relief=SUNKEN)
    win.frame3.pack(fill=X, padx=1, pady=1)
    imgpath         = os.path.expanduser('~') + '/PANorama2.0/icons/'
    zoomIn_img      = PhotoImage(file=imgpath+'zoomin.gif')
    zoomOut_img     = PhotoImage(file=imgpath+'zoomout.gif')
    Button(win.frame3, image=zoomIn_img,command=ZoomIn).pack(side=LEFT)
    Button(win.frame3, image=zoomOut_img,command=ZoomOut).pack(side=LEFT)
    Button(win.frame3, text='Quit',command=Quit).pack(side=RIGHT)

    win.mainloop()
