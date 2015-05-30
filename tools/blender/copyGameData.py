import os
import shutil

if os.path.exists(os.path.expanduser("~/.glportal/")):
    shutil.rmtree(os.path.expanduser("~/.glportal/"))
#os.makedirs(os.path.expanduser("~/.glportal/"))
shutil.copytree("../../data", os.path.expanduser("~/.glportal/data"))