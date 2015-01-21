__author__ = 'Yagel'

"""
Main target of server - Fastest and most secure service, clean from errors and problems and stable as much as possible.

Properties:

1.Multi-user server.
2.Will support SSL.
3.Will use connection class.

"""

import socket
import logging
import re
import sqlite3
import threading
from Connection import Connection
from PackageManager import PackageManager

PATH = "config.txt"
pm = None
users_db = None


def init(port, packs_db, users_db_path):
    global pm, users_db
    pm = PackageManager(packs_db)
    users_db = sqlite3.connect(users_db_path)
    listener = socket.socket()
    listener.bind(("", int(port)))
    listener.listen(1)
    while True:
        client = listener.accept()
        code, data = strip_message(client[0].recv(1024))
        if code == "100":
            user_id = login(data.split(",")[0], data.split(",")[1])
            if user_id:
                # logging.info("Client ")
                threading.Thread(target=protocol, args=(Connection(client[0], client[1]),)).start()
            else:
                client[0].send("901\r\n")
                client[0].close()
    # TODO: Add info logging.
    listener.close()

def readFromConfFile(path):
    try:
        handler = open(path, "rt")
        data = handler.read()
        handler.close()
        configs = re.findall(".+= (.+)\n", data)
        return configs
    except:
        return False


def login(user, password):
    cursor = users_db.cursor()
    try:
        user_id = cursor.execute("select user_id from users where user = ? and password == ?", (user, password, )).fetchone()
    except:
        return 1
    return user_id
    # Will add users db then change to zero in except.


def protocol(client):
    global pm
    client.send("101")
    code, data = strip_message(client.recv(1024))
    if code == "102":
        package = pm.get_package(int(data))
        client.send(package.get_info())
    client.close()


def strip_message(msg):
    return msg[:3], msg[3:-2]


def main():
    logger = logging.getLogger(__name__)
    logger.setLevel(20)
    logging.basicConfig(file="log.log", format='%(asctime)-15s %(clientip)s %(message)s')
    port, packs_db, users_db_path = readFromConfFile(PATH)
    init(port, packs_db, users_db_path)


if __name__ == "__main__":
    main()