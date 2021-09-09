import tkinter as tk
from tkinter import *

class GUI(tk.Tk):

    def __init__(self, *args, **kwargs):

        tk.Tk.__init__(self, *args, **kwargs)
        container = tk.Frame(self)

root=tk.Tk()
root.title("Main Page")
label1=tk.Label(root, text="Device 1")
label1.place(x=200,y=50)

label2=tk.Label(root, text="Device 2")
label2.place(x=350,y=50)

label3=tk.Label(root, text="Device 3")
label3.place(x=500,y=50)

label4=tk.Label(root, text="Device 4")
label4.place(x=650,y=50)

def toggle1():

    if button1.config('text')[-1] == 'ON':
        button1.config(text='OFF',fg='red')
    else:
        button1.config(text='ON',fg='green')

def toggle2():

    if button2.config('text')[-1] == 'ON':
        button2.config(text='OFF',fg='red')
    else:
        button2.config(text='ON',fg='green')

def toggle3():

    if button3.config('text')[-1] == 'ON':
        button3.config(text='OFF',fg='red')

    else:
        button3.config(text='ON',fg='green')

def toggle4():

    if button4.config('text')[-1] == 'ON':
        button4.config(text='OFF',fg='red')
    else:
        button4.config(text='ON',fg='green')

button1=tk.Button(text="ON",fg='green',command=toggle1)
button1.place(x=200,y=150)

button2=tk.Button(text="ON",fg='green',command=toggle2)
button2.place(x=350,y=150)

button3=tk.Button(text="ON",fg='green',command=toggle3)
button3.place(x=500,y=150)

button4=tk.Button(text="ON",fg='green',command=toggle4)
button4.place(x=650,y=150)

status_label=tk.Label(root, text="Status")
status_label.place(x=50,y=150)

health_label=tk.Label(root, text="Health")
health_label.place(x=50,y=250)

health_label1=tk.Label(root, text="Device 1")
health_label1.place(x=200,y=250)

health_label2=tk.Label(root, text="Device 2")
health_label2.place(x=350,y=250)

health_label3=tk.Label(root, text="Device 3")
health_label3.place(x=500,y=250)

health_label4=tk.Label(root, text="Device 4")
health_label4.place(x=650,y=250)


def create_window():
    window = tk.Toplevel(root)

setting_button=tk.Button(text="Settings",command=create_window)
setting_button.place(x=400,y=350)


root.mainloop()



