import tkinter as tk
from tkinter import *
import serial

SerialData = serial.Serial('com4', 9600)

class GUI(tk.Tk):

    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        container = tk.Frame(self)

        container.pack(side="top", fill="both", expand=True)

        container.grid_rowconfigure(0,weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames={}

        for F in (MainPage, Settings):

            frame = F(container,self)

            self.frames[F] = frame

            frame.grid(row = 0, column=0, sticky="nsew")

        self.show_frame(MainPage)

    def show_frame(self, cont):
        frame=self.frames[cont]
        frame.tkraise()

def toggle2(self):
    SerialData.write(b'1')
    if self.config('text')[-1] == 'ON':
        self.config(text='OFF',fg='red')
    else:
        self.config(text='ON',fg='green')

def toggle1(self):
    SerialData.write(b'2')
    if self.config('text')[-1] == 'ON':
        self.config(text='OFF',fg='red')
    else:
        self.config(text='ON',fg='green')

def toggle3(self):
    SerialData.write(b'3')
    if self.config('text')[-1] == 'ON':
        self.config(text='OFF',fg='red')
    else:
        self.config(text='ON',fg='green')

def toggle4(self):
    SerialData.write(b'4')
    if self.config('text')[-1] == 'ON':
        self.config(text='OFF',fg='red')
    else:
        self.config(text='ON',fg='green')

def PowerValues(self):
    temp=self.split(",")
    temp=[int(a) for a in temp]
    return temp


def update_val(self,master):
    # print(45)
    myData2 = SerialData.readline()
    myData3 = SerialData.readline()
    # print(myData)
    self.configure(text=myData2)
    master.configure(text=myData3)



class MainPage(tk.Frame):
    def __init__(self, parent, controller):
        myData = SerialData.readline()
        myData1 = SerialData.readline()

        tk.Frame.__init__(self,parent)

        label1 = tk.Label(self, text="Device 1")
        label1.grid(row = 0 , column = 1,padx=50,pady=20)

        label2 = tk.Label(self, text="Device 2")
        label2.grid(row = 0 , column = 2,padx=50)

        label3 = tk.Label(self, text="Device 3")
        label3.grid(row=0, column=3,padx=50)

        label4 = tk.Label(self, text="Device 4")
        label4.grid(row=0, column=4,padx=50)



        status_label = tk.Label(self, text="Status")
        status_label.grid(row=1, column=0,padx=50)

        button1 = tk.Button(self, text="OFF", fg='red', command=lambda: toggle1(button1))
        button1.grid(row=1, column=1)

        button2 = tk.Button(self, text="OFF", fg='red', command=lambda: toggle2(button2))
        button2.grid(row=1, column=2)

        button3 = tk.Button(self, text="OFF", fg='red', command=lambda: toggle3(button3))
        button3.grid(row=1, column=3)

        button4 = tk.Button(self, text="OFF", fg='red', command=lambda: toggle4(button4))
        button4.grid(row=1, column=4)



        #power = PowerValues("123,45,57,74")
        #power1, power2, power3, power4 = power

        power_label = tk.Label(self, text="Current")
        power_label.grid(row=2, column=0, pady=50)
        print(myData)
        power_label1 = tk.Label(self, text=myData)
        power_label1.grid(row=2, column=1)

        power_label2 = tk.Label(self, text=myData1)
        power_label2.grid(row=2, column=2)

        power_label3 = tk.Label(self, text="0.00")
        power_label3.grid(row=2, column=3)

        power_label4 = tk.Label(self, text=0.00)
        power_label4.grid(row=2, column=4)




        health_label = tk.Label(self, text="Health")
        health_label.grid(row=3, column=0)

        health_label1 = tk.Label(self, text="OK")
        health_label1.grid(row=3, column=1)

        health_label2 = tk.Label(self, text="OK")
        health_label2.grid(row=3, column=2)

        health_label3 = tk.Label(self, text="OK")
        health_label3.grid(row=3, column=3)

        health_label4 = tk.Label(self, text="OK")
        health_label4.grid(row=3, column=4)

        setting_button = tk.Button(self, text="Settings", command=lambda :controller.show_frame(Settings))
        setting_button.grid(row=4, column=4,pady=100)

        Update_button = tk.Button(self,text="Update Current",command=lambda: update_val(power_label1,power_label2))
        Update_button.grid(row=4,column=3)

class Settings(tk.Frame):


    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        Dv_label1 = tk.Label(self, text="Device1")
        Dv_label1.grid(row=0,column=0,padx=40,pady=40)

        Dv_label2 = tk.Label(self, text="Device2")
        Dv_label2.grid(row=1,column=0,pady=40)

        Dv_label3 = tk.Label(self, text="Device3")
        Dv_label3.grid(row=2,column=0,pady=40)

        Dv_label4 = tk.Label(self, text="Device4")
        Dv_label4.grid(row=3,column=0,pady=40)

        tkvar = StringVar(self)
        tkvar1 = StringVar(self)
        tkvar2 = StringVar(self)
        tkvar3 = StringVar(self)

        choices = {'Lamp', 'Fan', 'TubeLight', 'Oven'}

        tkvar.set('Lamp')
        tkvar1.set('Lamp')
        tkvar2.set('Lamp')
        tkvar3.set('Lamp')

        list1 = OptionMenu(self, tkvar, *choices)
        list1.grid(row=0,column=1)

        list2 = OptionMenu(self, tkvar1, *choices)
        list2.grid(row=1,column=1)

        list3 = OptionMenu(self, tkvar2, *choices)
        list3.grid(row=2,column=1)

        list4 = OptionMenu(self, tkvar3, *choices)
        list4.grid(row=3,column=1)

        PR_label1 = tk.Label(self, text="Power Rating")
        PR_label1.grid(row=0,column=2,padx=50)

        PR_label2 = tk.Label(self, text="Power Rating")
        PR_label2.grid(row=1,column=2)

        PR_label3 = tk.Label(self, text="Power Rating")
        PR_label3.grid(row=2,column=2)

        PR_label4 = tk.Label(self, text="Power Rating")
        PR_label4.grid(row=3,column=2)

        e1 = tk.Entry(self)
        e1.grid(row=0,column=3)

        e2 = tk.Entry(self)
        e2.grid(row=1,column=3)

        e3 = tk.Entry(self)
        e3.grid(row=2,column=3)

        e4 = tk.Entry(self)
        e4.grid(row=3,column=3)

        back_button = tk.Button(self, text="Back", command=lambda: controller.show_frame(MainPage))
        back_button.grid(row=4, column=3, pady=20)

app=GUI()
app.title("Energy Monitoring, Usage Optimization and Condition Monitoring System")
app.mainloop()
