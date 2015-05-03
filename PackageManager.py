__author__ = 'yagel'

import sqlite3

from Package import Package


class PackageManager:
    def __init__(self, db_path):
        self.packages = []
        self.db = db_path
        packages_info = self.read_from_db()
        for package in packages_info:
            self.packages.append(Package(package[0], package[1], package[2], package[3], package[4], package[5], package[6]))

    def get_package(self, id):
        for package in self.packages:
            if package.get_id() == id:
                return package

    def read_new_packages(self):
        packages = self.read_from_db()
        if len(packages) > len(self.packages):
            for package in packages:
                # self.packages.append(packages[i + len(self.packages)])
                self.packages.append(Package(package[0], package[1], package[2], package[3], package[4], package[5], package[6]))

    def read_from_db(self):
        db = sqlite3.connect(self.db)
        cursor = db.cursor()
        packs = cursor.execute("select * from packages").fetchall()
        db.close()
        return packs

pm = PackageManager("ServerPackagesDB")


def get_package_info(client, data):
    package = pm.get_package(int(data[0]))
    if not package:
        client.send("902\r\n")
    else:
        data = str(package.get_info())
        client.send("103\r\n" + data + "\r\n")


def send_package(client, data):
    try:
        package = pm.get_package(data[0])
        with open(package.get_path(), "rb") as r:
            data = r.read()
    except:
        return False
    client.send("105\r\n%s\r\n") % str(len(data))
    client.sendall(data)
    client.send("\r\n")

