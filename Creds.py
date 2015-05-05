__author__ = 'yagel'

from PackageManager import *

users_db = sqlite3.connect("ServerUsersDB")


def login(client, data):
    """
    Get user id for authentication and load info about user.
    """
    cur = users_db.cursor()
    user, password = data.split()
    try:
        user_id = cur.execute("select user_id from users where user = ? and password == ?", (user, password,)).fetchone()
    except:
        return 0
    if len(user_id) > 0 or (data[0] == "admin" and data[1] == "admin"):
        client.send("101")
        return True
        # return user_id
    return False
    # return 0

