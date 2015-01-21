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
            for i in range(len(packages) - len(self.packages)):
                self.packages.append(packages[i + len(self.packages)])

    def read_from_db(self):
        db = sqlite3.connect(self.db)
        cursor = db.cursor()
        packs = cursor.execute("select * from packages").fetchall()
        db.close()
        return packs
