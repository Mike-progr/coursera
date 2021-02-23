
from tkinter import *
import socket
import threading

PORT = 50000
HOST = "localhost"

s = socket.socket()    
s.bind((HOST,PORT))
s.listen(5)
conn, address = s.accept()

class server:

    def __init__(self):

        self.Window_srv = Tk()
        self.Window_srv.withdraw()

        self.login = Toplevel()
        self.login.title("Login")
        self.login.geometry("500x250")
        
        self.title = Label(self.login, text = "Server Login", fg="black",font=("Helevetica",20,"bold"))
        self.title.place(x=150,y=30)

        self.usname = Label(self.login, text = "username:", fg="black",font=("Helevetica",16))
        self.usname.place(x=100,y=100)

        self.psswrd = Label(self.login, text = "password:", fg="black",font=("Helevetica",16))        
        self.psswrd.place(x=100,y=130)

        self.username = Entry(self.login,text = "")
        self.username.place(x=205,y=105)
        
        self.password = Entry(self.login, text = "")
        self.password.place(x=205,y=135)
        
        self.btn_go = Button(self.login ,text = "Go", fg = "black",bg = "blue" , width = 3, height = 1, font=("Helevetica",16,"bold"),command=lambda: self.check_PassUsenam()) 
        self.btn_go.place(x = 170, y = 180)

        self.Window_srv.mainloop()

    def check_PassUsenam(self):

        if(self.username.get() == "server" and self.password.get() == "123456"):

            self.login.destroy()
            self.layout()

            rcv = threading.Thread(target=self.receive)
            rcv.start()

        else:

            self.msg = Label(self.login, text = "*Please try again", fg="red",font=("Helevetica",12,"bold"))
            self.msg.place(x=150,y=65)

            self.username.delete(0,END)
            self.password.delete(0,END)

    def layout(self):
        
        self.Window_srv.deiconify()
        self.Window_srv.title("Server")
        self.Window_srv.geometry("525x500")
        
        self.text_read = Text(self.Window_srv, bg = "powderblue", width=65, height=20)
        self.text_read.place(x=0, y=0)
        self.text_read.configure(state='disabled')

        self.scrollbar = Scrollbar(self.text_read)
        self.scrollbar.place(relheight = 1, relx = 0.974)
        self.scrollbar.config(command = self.text_read.yview)

        self.text_write = Text(self.Window_srv, width = 30, height = 5)
        self.text_write.place(x = 100, y = 350)

        self.btn_send = Button(self.Window_srv ,text = "Send→", fg = "black",bg = "green" , width = 5, height = 1, font=("Helevetica",16,"bold"), command = lambda: self.send())
        self.btn_send.place(x = 400, y = 370)

    def receive(self):

        while True:

             data = conn.recv(1024).decode("utf-8")

             self.text_read.configure(state = NORMAL)                   
             self.text_read.insert(INSERT, "Client:" + data + "\n")
             self.text_read.configure(state = DISABLED)

    def send(self):

        conn.send(self.text_write.get("1.0",END).encode("utf-8"))
        self.text_write.delete('1.0', END)

srv = server()
