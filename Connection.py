__author__ = 'yagel'


import time


class Connection:
    def __init__(self, sock, details):
        self.sock = sock
        self.details = details
        self.last_act = time.time()

    def send(self, message):
        self.last_act = time.time()
        self.sock.send(message)

    def recv(self, len):
        self.last_act = time.time()
        return self.sock.recv(len)

    def isActive(self):
        return not (time.time() - self.last_act > 120)

    def close(self):
        self.sock.close()
