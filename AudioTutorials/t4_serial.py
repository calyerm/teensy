# -*- coding: utf-8 -*-
"""
Description : Basic serial interface 
              115200 8 N 1
Created on Sun Apr 26 15:10:42 2020
author: mcalyer
"""
import sys
import time
import serial

class Serial_Comm:
    def __init__(self):  
        self.uart = None  
        self.port = None        

    def connect(self, port = None):
        if self.uart:
            return 0  
        if port is None : port = self.port        
        # Open serial port connection
        # port is a string based on OS:
        # Examples: Windows 'COM12' , Linux:  '/dev/ttyACM0'
        try:   
            self.uart = serial.Serial(port, baudrate=115200, timeout=1)   
            self.port = port            
            return 0 , None           
        except:           
            self.uart = None  
            self.port = None            
            return 1 , 'Serial port connection error !'        
     
    def disconnect(self):
        if self.uart:            
            self.uart.close()
            self.uart = None       
       
    def tx(self,cp):       
        try:
            #c = ''.join(e for e in cmd_list)
            c = cp.encode()           
            self.uart.write(c)    
            #self.uart.write(cmd_list)               
            return 0 , None
        except serial.SerialException:         
            return 1 ,'Command: Serial Port Failed'  
            
    def rx(self, n, delay = 0):          
        if delay != 0 : time.sleep(delay)      
        try:           
            nb = self.uart.inWaiting()   
            #print(nb)            
            if nb == 0: return 1 , 'RxBytes: Zero serial bytes'   
            if n == '!': n = nb                    
            if n != nb:  
                self.uart.flush()            
                return 1 , 'RxBytes: Expected : ' + str(n) + ' Received : ' + str(nb)      
            data = self.uart.read(n)           
            return 0 , data            
        except serial.SerialException:              
            return 1, 'RxBytes: Serial Port Failed'  
  
        
    def flush(self):        
        self.uart.flush()              
            
serial_comm =  Serial_Comm()

