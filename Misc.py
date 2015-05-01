__author__ = 'yagel'

def strip_message(data):
    """
    Strips the message to MC and data.
    :param data: Data before strip.
    :return: Tuple of stripped data.
    """
    data = data.split()
    code = data[0]
    data = data[1:]
    return code, data


def check_request(code, data):
    return True
    # TODO: Check Request.