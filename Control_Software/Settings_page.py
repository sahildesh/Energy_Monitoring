import tkinter as tk
from tkinter import *

root1=tk.Tk()
root1.title("Settings")

Dv_label1=tk.Label(root1, text="Device1")
Dv_label1.place(x=100,y=100)

Dv_label2=tk.Label(root1, text="Device2")
Dv_label2.place(x=100,y=200)

Dv_label3=tk.Label(root1, text="Device3")
Dv_label3.place(x=100,y=300)

Dv_label4=tk.Label(root1, text="Device4")
Dv_label4.place(x=100,y=400)

tkvar = StringVar(root1)
tkvar1 = StringVar(root1)
tkvar2 = StringVar(root1)
tkvar3 = StringVar(root1)

choices = {'Lamp','Fan','TubeLight','Oven'}
tkvar.set('Lamp')
tkvar1.set('Lamp')
tkvar2.set('Lamp')
tkvar3.set('Lamp')

list1=OptionMenu(root1,tkvar,*choices)
list1.place(x=200,y=100)

list2=OptionMenu(root1,tkvar1,*choices)
list2.place(x=200,y=200)

list3=OptionMenu(root1,tkvar2,*choices)
list3.place(x=200,y=300)

list4=OptionMenu(root1,tkvar3,*choices)
list4.place(x=200,y=400)


PR_label1=tk.Label(root1, text="Power Rating")
PR_label1.place(x=500,y=100)

PR_label2=tk.Label(root1, text="Power Rating")
PR_label2.place(x=500,y=200)

PR_label3=tk.Label(root1, text="Power Rating")
PR_label3.place(x=500,y=300)

PR_label4=tk.Label(root1, text="Power Rating")
PR_label4.place(x=500,y=400)


e1=Entry()
e1.place(x=600,y=100)

e2=Entry()
e2.place(x=600,y=200)

e3=Entry()
e3.place(x=600,y=300)

e4=Entry()
e4.place(x=600,y=400)


root1.mainloop()