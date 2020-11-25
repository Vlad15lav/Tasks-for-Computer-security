from factor.lenstra import *
from tkinter import *
from win32com.client import GetObject

# Use win32com.client for get CPU Name on current PC
root_winmgmts = GetObject("winmgmts:root\cimv2")
cpus = root_winmgmts.ExecQuery("Select * from Win32_Processor")

# Method for click event
def btn_click():
    N = int(EntryN.get())
    B = int(EntryB.get())
    Curves = int(EntryC.get())

    result = Lenstra(N, B, Curves)
    BoxOutput.delete(1.0, END)
    if result is None:
        BoxOutput.insert(1.0, 'Factorization is fill.\nTry more B or more number of curves.')
        return
    txt = 'Factorization - {},\n' \
          'Tried {} curves,\n' \
          'Point - P({}, {})\n' \
          'Curve - Y^3 = X^3+({})X+({})\Time factorization - {} second'.\
        format(result['Factorization'], result['Curves'], result['X'], result['Y'], result['A'], result['B'], result['time'])
    BoxOutput.insert(1.0, txt)

# Form
root = Tk()
root.title('Factorization Lenstra')
root['bg'] = '#fafafa'
root.geometry('500x400')

root.resizable(width=False, height=False)

canvas = Canvas(root, height=500, width=400)
canvas.pack()

frame = Frame(root, bg='#fafafa')
frame.place(relx=0, rely=0, relwidth=1, relheight=1)

# Fild for N
labelN = Label(frame, text='Entry N', bg='#fafafa', font=20)
labelN.place(x=20, y=20)
EntryN = Entry(frame, width=40, justify='center')
EntryN.place(x=120, y=24)

# Fild for B
labelB = Label(frame, text='Entry B', bg='#fafafa', font=20)
labelB.place(x=20, y=60)
EntryB = Entry(frame, width=40, justify='center')
EntryB.place(x=120, y=64)

# Fild for number of curves
labelC = Label(frame, text='Entry num curves', bg='#fafafa', font=20)
labelC.place(x=20, y=100)
EntryC = Entry(frame, width=25, justify='center')
EntryC.place(x=210, y=104)

# Button call the factorization Lentra
btn = Button(frame, text='Factorization', bg='gray', command=btn_click)
btn.place(x=390, y=60)

# CPU info
labelCPU = Label(frame, text='Info about CPU: {}'.format(cpus[0].Name), bg='#fafafa', font=20)
labelCPU.place(x=20, y=144)

# Output
BoxOutput = Text(frame, width=56, height=12)
BoxOutput.place(x=25, y=180)

root.mainloop()