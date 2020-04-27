# -*- coding: utf-8 -*-
"""
File : t4_ctrl_panel.py
Date : 04/26/2020
Author : mcalyer
Description : Virtual Panel for Teensy Audio Tutorials , Virtual pots and buttons replacing breadboard hardware
              Implemented with Python 3.7 , tk application sending data , teensy RX/TX USB serial port
              Teensy Audio Tutorials will need to be modified based on the code below , replacing/modifing button and analogin function 
              
Format : Integer data A0 SP A1 SP A2 SP B0 SP B1 SP B2 '\n'    
         Where: SP = " " , space
                A0,A1,A2 range  :  0 -1023
                B0,B1,b2 values :  1,0



"""

import tkinter as tk
from tkinter import ttk
from tkinter import filedialog

import serial
import time
from t4_serial import *

class Control_Panel():
    def __init__(self,sp,A1='A1', A2='A2', A3='A3', B0='B0', B1='B1', B2='B2'):
        # Serial interface
        self.port = None
        self.sp = sp
        # Window
        self.window = tk.Tk()
        self.window.title("Teensy Control Panel")
        self.window.geometry("600x600")
        self.window['padx'] = 5
        self.window['pady'] = 5 
        self.window.resizable(False, False)
        #self.window.protocol("WM_DELETE_WINDOW", self.program_exit)
        # Status Bar
        self.window.status_bar = None
        self.status = tk.StringVar()
        #self.status_bar('Mike')
        # Frame
        self.frame = ttk.LabelFrame(self.window, relief=tk.RIDGE)
        self.frame.grid(rowspan=8, columnspan=2, sticky=tk.W)
        # Text
        self.log_window = None
        # Widgets
        self.create_widgets(A1,A2,A3,B0,B1,B2)
        self.A1_value = 0
        self.A2_value = 0
        self.A3_value = 0
        self.B0_value = 0
        self.B1_value = 0
        self.B2_value = 0  
        self.status_bar()           
        
    def status_bar(self,port = None,port_status = None):
        sb = ''
        ps = ''
        if port is not None:
            ps = F"{port} : {port_status}"            
        n = 100 - len(ps)
        sb = ps + (n * ' ')
        self.status.set(sb)        
    
    def program_exit(self):
    # Not implemented
       if self.port is None : return
       self.sp.disconnect()
       
    def connect(self,port):
        self.port = port
        r,m = self.sp.connect(port)         
        if r :
            self.port = None  
            self.status_bar(port,'NOT CONNECTED')   
            return r, m
        self.status_bar(port,'CONNECTED')       
        return r , m
        
    def tx(self,rx=False):
        if self.port is None : return
        SP = " "
        cp = str(self.A1_value) + SP +  \
             str(self.A2_value) + SP +  \
             str(self.A3_value) + SP +  \
             str(self.B0_value) + SP +  \
             str(self.B1_value) + SP +  \
             str(self.B2_value) + "\n"
        self.sp.tx(cp)        
        # RX
        if rx:
            result , message = self.sp.rx('!',.020)
            self.log_window.insert(tk.END,message)       
        
        
    def create_widgets(self,A1,A2,A3,B0,B1,B2):       
        
        # Pots
        A1_Pot = tk.Scale(self.frame, from_=0, to=1023, label=A1 , command = self.A1_Update, orient=tk.HORIZONTAL, width=8, length=400)
        A1_Pot.grid(row=3, column=0, sticky=tk.W)

        A2_Pot = tk.Scale(self.frame, from_=0, to=1023, label=A2 , command = self.A2_Update, orient=tk.HORIZONTAL, width=8, length=400)
        A2_Pot.grid(row=4, column=0, sticky=tk.W)
        
        A3_Pot = tk.Scale(self.frame, from_=0, to=1023, label=A3 , command = self.A3_Update, orient=tk.HORIZONTAL, width=8, length=400)
        A3_Pot.grid(row=5, column=0, sticky=tk.W)        
        
        # Buttons      
        button_0 = ttk.Button(self.frame, text=B0 , command=self.B0_Update)
        button_0.grid(row=3, column=1, padx = 20)
        
        button_1 = ttk.Button(self.frame, text=B1 , command=self.B1_Update)
        button_1.grid(row=4, column=1, padx = 20)
        
        button_2 = ttk.Button(self.frame, text=B2 , command=self.B2_Update)
        button_2.grid(row=5, column=1, padx = 20)
        
        button_clear = ttk.Button(self.frame, text='Clear', command=self.text_clear)
        button_clear.grid(row=7, column=1,pady = 40)
        
        # Text
        self.log_window = tk.Text(self.frame, height = 16 , width = 48)
        self.log_window.grid(row=7, column=0, pady = 20)
        
        # Status Bar
        self.window.status_bar = tk.Label(self.frame, textvariable = self.status, bd = 1, relief=tk.SUNKEN , anchor=tk.W)
        self.window.status_bar.grid(row=8, column=0, pady = 20)
        
    def text_clear(self):
        self.log_window.delete('1.0',tk.END) 
        
    def A1_Update(self,value):         
          self.A1_value = value  
          self.tx()           
        
    def A2_Update(self,value):
          self.A2_value = value  
          self.tx()       
          
    def A3_Update(self,value):
          self.A3_value = value 
          self.tx()          
        
    def B0_Update(self): 
          self.B0_value = 1   
          self.tx(True)        
          self.B0_value = 0          
        
    def B1_Update(self):
          self.B1_value = 1 
          self.tx(True)          
          self.B1_value = 0           
          
    def B2_Update(self):
          self.B2_value = 1
          self.tx(True)        
          self.B2_value = 0
         

# Create the entire GUI program
ctrl_panel = Control_Panel(serial_comm,'Volume','Frequency','FM Frequency','Waveform','Source','Envelope')
r , m = ctrl_panel.connect('COM65')
#if r : print(m) ; exit(0)

# Start the GUI event loop
ctrl_panel.window.mainloop()
