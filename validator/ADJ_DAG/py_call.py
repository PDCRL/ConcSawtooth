
from ctypes import cdll
from ctypes import c_int

# load the library
lib = cdll.LoadLibrary('./libgeek.so')



# create a Geek class
class Geek(object):
  
    # constructor
    def __init__(self):
  
        # attribute
        self.obj = lib.Geek_new()
  

    # define method
    def DAG_prune(self):
        lib.DAG_prune(self.obj)

    # define method
    def DAG_create(self):
        lib.DAG_create(self.obj)

    # define method
    def DAG_create2(self):
        lib.DAG_create2(self.obj)


    # define method
    def DAG_select(self):
        return lib.DAG_select(self.obj)

	

    # define method
    def DAG_delete(self, int):
        lib.DAG_delete(self.obj, int )
  
# create a Geek class object
f = Geek()

f.DAG_create()
f.DAG_create()
f.DAG_create2()
x=f.DAG_select()
print(x)
f.DAG_delete(x)


