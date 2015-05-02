__author__ = 'yagel'

import socket

from Creds import *
from Misc import *
from PackageManager import *


PORT = 4567
funcs = {"100": login, "102": get_package_info, "104": send_package}  # The new way to create servers.


def init(port):
    listener = socket.socket()
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind(("", port))
    listener.listen(5)
    client = listener.accept()[0]
    listener.close()
    return client


def serve(client):
    while True:
        try:
            code, data = strip_message(client.recv(1024))
        except:
            client.close()
            print "Client disconnected."
            client = init(PORT)
            continue
        try:
            if check_request(code, data):
                funcs[code](client, data)
            else:
                # TODO Rejection mc.
                pass
        except:
            # TODO Rejection mc.
            pass


def main():
    client = init(PORT)
    serve(client)


if __name__ == "__main__":
    main()
