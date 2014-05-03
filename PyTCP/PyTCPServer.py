# -*- coding: utf-8 -*-
"""
Created on Thu Dec 05 15:25:55 2013

@author: jeff.zignego
"""

import socket
import threading
#import traceback
import msvcrt as m

# setup function
def setup():
    TCP_IP = '' # socket will be reachable regardless of server's IP
    TCP_PORT = 5005 # our applications server port

    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serverSocket.bind((TCP_IP, TCP_PORT))
    return serverSocket

# receive the clients info and echo it back
def recvWorker(socket, summonState):
    #try:
        data = clientSocket.recv(1024)
        if not data:
            return

        if(summonState == False):
            clientSocket.send(data)  # echo, the data to the client
            #sclientSocket.shutdown(socket.SHUT_RDWR)
            clientSocket.close() # close the connection, the client needs to open a new one

    #except Exception as e:
    #    print e
    #    traceback.print_exc()

# handle a clients timer
def handleClientTimer(clientIP, clientDictionary):
    print "The client: " + str(clientIP) + " has not contacted the Shepherd in 10 seconds"
    newClientTimer = threading.Timer(10, handleClientTimer, [clientIP, clientDictionary])
    newClientTimer.start()
    clientDictionary[clientIP] = newClientTimer

clientDictionary = {}
summonState = False
# main server loop
while True:
    #try:
        serverSocket = setup()
        serverSocket.listen(5)

        (clientSocket, (clientIP, clientPort)) = serverSocket.accept()

        # if this is not a new client, kick his timer
        if( clientIP in clientDictionary.keys() ):
            oldClientTimer = clientDictionary[clientIP]
            oldClientTimer.cancel()
            newClientTimer = threading.Timer(10, handleClientTimer, [clientIP, clientDictionary])
            newClientTimer.start()
            clientDictionary[clientIP] = newClientTimer
        # if this is a new client, add him to the clientList
        # and do not kick his timer
        else:
            newClientTimer = threading.Timer(10, handleClientTimer, [clientIP, clientDictionary])
            newClientTimer.start()
            clientDictionary[clientIP] = newClientTimer
            print "A new client connected to the Shepherd. The client address is: " + str(clientIP)
        clientThread = threading.Thread(target=recvWorker, args = (clientSocket, summonState))
        clientThread.start()

        if(m.kbhit()):
            input = m.getch()
            summonState = not (summonState)

            if(summonState):
                print "Clients have been summoned."
            else:
                print "The summon command has been turned off."

    #except Exception as e:
    #    print e
    #    traceback.print_exc()