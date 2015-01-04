__author__ = 'Yagel'

"""
Main target of server - Fastest and most secure service, clean from errors and problems and stable as much as possible.

Properties:

1.Only for one client.
2.Will support SSL.
3.Will use connection class.

"""

import socket
import time
import logging
import re
import Connection


PATH = "config.txt"


def init(port):
    listener = socket.socket()
    listener.bind(("", port))
    listener.listen(1)
    client = listener.accept()
    listener.close()
    # TODO: Add info logging.
    return client[0]


def readFromConfFile(path):
    try:
        handler = open(path, "rt")
        data = handler.read()
        handler.close()
        configs = re.findall(".+= (.+)\n", data)
        return configs
    except:
        return False


def protocol(client):
    pass


def main():
    # ToDo: logging.basicConfig(file = "log.log") - Add format string.
    port = readFromConfFile(PATH)  # Here we can add more when we will need them, e.g. - workspace.
    client = init(port)
    protocol(client)

