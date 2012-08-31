import version
import os
import sys
import subprocess

IsPython3 = version.IsPython3

if  "3" == IsPython3 :
    #from  tkinter import  tkinter
    from  tkinter  import *
elif "2" == IsPython3:
    from  Tkinter  import *

__metaclass__ = type
class BuildUI:
    "The UI of build"
    def __init__(self,width=320, height=240) :
        self.__mainFrame(width, height)

    def __mainFrame(self, width, height):
        self.root=Tk()
        self.root.resizable(False,False) 
        self.root.title("setup")
       
        screennWidth,screennHeight = self.root.maxsize() 
        if (screennWidth<width):
            width = screennWidth/2;
        if (screennHeight<height):
            height = screennHeight/2;
        tmpcnf = '%dx%d+%d+%d'%(width, height, (screennWidth-width)/2,(screennHeight-height)/2) 
        self.root.geometry(tmpcnf)
        self.root.update()
        #label
        offset_X = self.root.winfo_width()/20          		
        label= Label(self.root, text="Configuration")
        label.place(x=offset_X, y=0);
        label.update()
        offset_Y =  label.winfo_height()
        
        #configuration frame1
        width = width * 9 / 10
        height = height *2/6 
        frame = Frame(self.root,width=width, height=height,bg="#%02x%02x%02x"%(128, 128, 128), borderwidth=10)        
        frame.place(x=offset_X, y=offset_Y)
        frame.update()
        self.__ConfigFrame(frame)
       
        offset_Y += frame.winfo_height()
        #customer frame
        label= Label(self.root, text="Compiler for windows")
        label.place(x=offset_X, y=offset_Y);
        label.update()
        offset_Y += label.winfo_height()
        frame = Frame(self.root,width=width, height=height,bg="#%02x%02x%02x"%(128, 128, 128), borderwidth=10)        
        frame.place(x=self.root.winfo_width()/20, y= offset_Y)
        frame.update()
        self.__CompilerFrame(frame)

        #button
        offset_X = self.root.winfo_width()/2
        offset_Y = self.root.winfo_height()-32
        b = Button(self.root, text="OK", command=self.__okButton, borderwidth=2, width=4)
        b.place(x=offset_X, y= offset_Y)
        b.update()
        offset_X += b.winfo_width()
        b = Button(self.root, text="Exit", command=self.__exitButton, borderwidth=2, width=4)
        b.place(x=offset_X, y=offset_Y)
        return

    def __ConfigFrame(self, frame) :
        frame.update()
        self.__IS_SHARED_LIBRARY = StringVar()
        self.__IS_SHARED_LIBRARY.set("1")

        #radio button need to bundle with a persist value to a group
        rb0 = Radiobutton(frame, text="Library Static", variable= self.__IS_SHARED_LIBRARY, value="0", width=12, anchor=W)
        rb0.place(x=2, y= frame.winfo_height()/10)
        rb0.update()    
        rb1 = Radiobutton(frame, text="Library Shared", variable= self.__IS_SHARED_LIBRARY, value="1", width=12, anchor=W)
        rb1.place(x=2, y= rb0.winfo_height()+frame.winfo_height()/10)
        
        #checkbox
        self.__OCL_ENABLE = StringVar()
        self.__OCL_ENABLE.set("0")       
        self.__OGL_ENABLE = StringVar()
        self.__OGL_ENABLE.set("0")
        cb0 = Checkbutton(frame, text="OCL_ENABLE", variable=self.__OCL_ENABLE, onvalue="1", offvalue="0", width=12)
        cb0.place(x=frame.winfo_width()/2 , y=frame.winfo_height()/10)       
        cb0.update()
        cb1 = Checkbutton(frame, text="OGL_ENABLE", variable=self.__OGL_ENABLE, onvalue="1", offvalue="0", width=12)
        cb1.place(x=frame.winfo_width()/2 , y=frame.winfo_height()/10 + cb0.winfo_height() )
        return

    def __CompilerFrame(self, frame) :
        frame.update()
        self.__COMPILER = StringVar()
        self.__COMPILER.set(" ")
        
        offset_Y = frame.winfo_height()/10
        offset_X = frame.winfo_width()/30
        bWidth = 12
        if "posix" == os.name:
            state = DISABLED
        else :
            state = NORMAL
        #radio button need to bundle with a persist value to a group
        rb = Radiobutton(frame, text="VC6", variable= self.__COMPILER, value="VC6", width=bWidth, anchor=W, state=state)
        rb.place(x=offset_X, y= offset_Y)
        rb.update()
        offset_Y += rb.winfo_height();     
        rb = Radiobutton(frame, text="VC2005", variable= self.__COMPILER, value="VC2005", width=bWidth, anchor=W, state=state)
        rb.place(x=offset_X, y= offset_Y)
        rb.update()
        offset_X += rb.winfo_width()
        offset_Y -= rb.winfo_height()
        rb = Radiobutton(frame, text="VC2008", variable= self.__COMPILER, value="VC2008", width=bWidth, anchor=W, state=state)
        rb.place(x=offset_X, y= frame.winfo_height()/10)
        rb.update()    
        offset_Y += rb.winfo_height();     
        rb = Radiobutton(frame, text="VC2010", variable= self.__COMPILER, value="VC2010", width=bWidth, anchor=W, state=state)
        rb.place(x=offset_X, y=offset_Y)
        return

    def __getCMakeStr(self):
        cmake_str="cmake "
        #compiler
        compiler_str = self.__COMPILER.get()
        if("VC6" == compiler_str) :
            cmake_str += ' -G"Visual Studio 6" '
        elif("VC2005" == compiler_str):
            cmake_str += ' -G"Visual Studio 8 2005" '
        elif("VC2008" == compiler_str):
            cmake_str += ' -G"Visual Studio 9 2008" '
        elif("VC2010" == compiler_str):
            cmake_str += ' -G"Visual Studio 10" '
        else :
            cmake_str += " " 

        if "1" == self.__IS_SHARED_LIBRARY.get():
            cmake_str += " -D SHARED_LIBRARY=on "
        if "1" == self.__OCL_ENABLE.get():
            cmake_str += " -D OCL_ENABLE=on "
        if "1" == self.__OGL_ENABLE.get():
            cmake_str += " -D OGL_ENABLE=on "
        cmake_str += " .."
        return cmake_str

    def __delete_file_folder(self, path):
        '''delete files and folders'''
        if os.path.isfile(path):
            try:
                os.remove(path)
            except:
                pass
        elif os.path.isdir(path):
            for item in os.listdir(path):
                itemsrc=os.path.join(path,item)
                self.__delete_file_folder(itemsrc) 
            try:
                os.rmdir(path)
            except:
                pass

    def __build(self):
        cmake_str = self.__getCMakeStr()
        print(os.getcwd())
        os.chdir("../")
        if os.path.exists("build"):
            self.__delete_file_folder("build")
        os.mkdir("build")
        os.chdir("./build")
        print(os.getcwd())
        subprocess.call(cmake_str, shell=True)
        print("Build Finished")
        return

    def __okButton(self) :
        self.__build()
        
    def __exitButton(self):
        os._exit(0)	
### UI variables
    root  = None
    __IS_SHARED_LIBRARY = None
    __OCL_ENABLE = None	
    __OGL_ENABLE = None
    __COMPILER = None
   
   




###

#entry = Tkinter.Entry(root, x=70, y=80)
#entry.pack()
#frame1=Tkinter.Frame(root,width=200, height=100, bg="green")
#frame1.pack()
#button1 = Tkinter.Button(root, width=5, height=2, text="ok1", bg="red")
#button1.pack()



    

