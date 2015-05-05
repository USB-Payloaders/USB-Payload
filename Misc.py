__author__ = 'yagel'
# fixed by omer 5/5
from Creds import *

def strip_message(data):
    """
    Strips the message to MC and data.
    :param data: Data before strip.
    :return: Tuple of stripped data.
    """
    data = data.split(",")
    code = data[0]
    data = data[1:]
    return code, data


def check_request(code, data):
    if code == "100" and len(data) == 2:
        return login(code, data)
    if code == "102" and len(data) == 1:
        return True
    if code == "104" and len(data) == 1:
        return True
    else:
        return False
