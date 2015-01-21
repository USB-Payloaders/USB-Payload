__author__ = 'yagel'


class Package:
    def __init__(self, ID, name, pack_type, isAlone, path, version, description=""):
        self.ID = ID
        self.name = name
        self.type = pack_type
        self.isAlone = isAlone
        self.path = path
        self.version = version
        self.description = description

    def get_id(self):
        return self.ID

    def get_name(self):
        return self.name

    def get_type(self):
        return self.type

    def isAlone(self):
        return self.isAlone

    def get_path(self):
        return self.path

    def get_ver(self):
        return self.version

    def get_info(self):
        return self.description