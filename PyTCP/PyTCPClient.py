# -*- coding: utf-8 -*-
"""
Created on Thu Dec 05 15:25:55 2013

@author: jeff.zignego
"""

import socket
import time

lfsr = 0xACE1 #initial starting value from wikipedia
TAPS = 0xB400 #taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1

def rand():
    global lfsr
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & TAPS)   
    rand = lfsr
    return rand

TCP_IP = '192.168.1.23'
TCP_PORT = 5005
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

while 1:
    MESSAGE = str(rand())
    s.send(MESSAGE)
    
    data = s.recv(BUFFER_SIZE)
    print "received data:", data
    
    time.sleep(10)

s.close()
