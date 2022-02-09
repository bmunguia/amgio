''' 
 
   Example of using Anisotropic Mesh Adaptation with pyAMG module 

   To get the module and conditions of use, visit https://pyamg.saclay.inria.fr
   
   Adrien Loseille, INRIA 2018
   Adrien.Loseille[@]inria.fr

'''
import numpy as np

# make sure you 
import pyamg 
import math


''' 
 
   Examples of utility functions to define an analitycal functional or a sizing-field functional 

'''

def analytical_fun(x,y,z=1):
    xyz = (x-0.4)*(y-0.4)*(z-0.4)
    if( xyz  <= (-1.*math.pi/50.)): return 0.1*math.sin(50.*xyz)
    elif(xyz <= 2.*math.pi/50.):    return     math.sin(50.*xyz)
    else:                           return 0.1*math.sin(50.*xyz) 

         
def create_sensor(mesh):
    sensor = []
    crd    = []
    if   ( 'xyz' in mesh ): 
        crd = mesh['xyz']
        for x in crd:
            sensor.append(analytical_fun(x[0],x[1],x[2]))
    elif ( 'xy' in mesh ):
        crd = mesh['xy']  
        for x in crd:
            sensor.append(analytical_fun(x[0],x[1]))
    return sensor


def analytical_sizing(x,y,z=1):
    d    = math.sqrt(x*x + y*y + z*z)
    hmin = 0.01
    hmed = 0.1
    hmax = 0.5
    if  ( d <= 10*hmin         ): return hmin
    elif( d <= 10*hmin+ 5*hmed ): return hmed
    else:                         return hmax 


def create_sizing(mesh):
    size = []
    crd    = []
    if   ( 'xyz' in mesh ): 
        crd = mesh['xyz']
        for x in crd:
            size.append(analytical_sizing(x[0],x[1],x[2]))
    elif ( 'xy' in mesh ):
            size.append(0.2)
    return size





# The list of functions in the module are 
print "\nFunctions included in pyamg, type help(pyamg) to display help : "
print dir(pyamg)
print "\n"

# IO routines, in pyamg, meshes are described as dictionary
msh3d = pyamg.read_mesh("cube.meshb")
msh2d = pyamg.read_mesh("unit.mesh")

print "\n Example of a 3D mesh dictionary loaded with msh3d = pyamg.read_mesh('cube.meshb') "
if ( 'xyz'        in msh3d ): print " array of 'vertices'    ",msh3d['xyz'].shape
if ( 'Triangles'  in msh3d ): print " array of 'triangles'   ",msh3d['Triangles'].shape
if ( 'Tetrahedra' in msh3d ): print " array of 'tetrahedra'  ",msh3d['Tetrahedra'].shape
if ( 'Edges'      in msh3d ): print " array of 'edges'       ",msh3d['Edges'].shape
if ( 'Corners'    in msh3d ): print " array of 'corners'     ",msh3d['Corners'].shape

print "\n Example of a 2D mesh dictionary loaded with msh2d = pyamg.read_mesh('unit.mesh') "
if ( 'xy'         in msh2d ): print " array of 'vertices'    ",msh2d['xy'].shape
if ( 'Triangles'  in msh2d ): print " array of 'triangles'   ",msh2d['Triangles'].shape
if ( 'Tetrahedra' in msh2d ): print " array of 'tetrahedra'  ",msh2d['Tetrahedra'].shape
if ( 'Edges'      in msh2d ): print " array of 'edges'       ",msh2d['Edges'].shape
if ( 'Corners'    in msh2d ): print " array of 'corners'     ",msh2d['Corners'].shape


# Note : to create your own dictionary you can use oython list directly (if numpy arrays are used on output, you can use for instance mesh['xyz'].tolist() 
# to have a list instead of a numpy array
# example of simple dictionary 
# mesh = {}
# mesh['xy']        = [ [0,0], [1,0], [1,1], [0,1] ]
# mesh['triangles'] = [ [1,2,3,0],[3,4,1,0]]
# pyviz.show(mesh)



print "\n------------------------------------"
print "                                      "
print " Example of uniform mesh refinement   "
print "                                      "
print "--------------------------------------"

# example of uniform refinment 
remesh_options = {}
remesh_options['hmax'] = 0.1
remesh_options['logfile'] = "remesh-unif.log"
remesh_options['options'] = "-adap-surf-ids 1-5 -gmem 0.1 "


print " remeshing options : "
print remesh_options
print "\n remeshing done with msh3d_unif = pyamg.adapt_mesh(msh3d, remesh_options) \n"


msh3d_unif = pyamg.adapt_mesh(msh3d, remesh_options)


print "\n output : cube_unif.meshb \n"   
pyamg.write_mesh(msh3d_unif,"cube_unif.meshb")



print "\n---------------------------------------------------------------------------"
print "                                                                           "
print " Example of mesh adaptation to control interpolation error on a function   "
print " ==> create the key 'sensor'                                               "
print "                                                                           "
print "---------------------------------------------------------------------------"

# 
remesh_options              = {}
remesh_options['Lp']        = 2
remesh_options['gradation'] = 1.5
remesh_options['target']    = 20000
remesh_options['options']   = " -gmem 0.1  "
#remesh_options['logfile']   = "remesh-adap-sensor.log"


print " remeshing options : "
print remesh_options
print "\n remeshing done with : msh3d_aniso           = pyamg.adapt_mesh(msh3d_aniso, remesh_options) \n"

# create the sensor for adaptation
msh3d['sensor'] = create_sensor(msh3d)


# adapt the mesh, perform 5 iterations
print  "   Anisotropic adaptation : iteration 1"
msh3d_aniso = pyamg.adapt_mesh(msh3d, remesh_options)

for ite in range(2,5):    
  print  "   Anisotropic adaptation : iteration %d " %(ite)    
  msh3d_aniso['sensor'] = create_sensor(msh3d_aniso)
  msh3d_aniso           = pyamg.adapt_mesh(msh3d_aniso, remesh_options)
  
print " \n output : cube_aniso.meshb \n"      
pyamg.write_mesh(msh3d_aniso,"cube_aniso.meshb")        



print "\n-------------------------------------------------------"
print "                                                         "
print " Example of mesh adaptation according to a sizing field  "
print " ==> create the key 'metric'                             "
print "                                                         "
print "---------------------------------------------------------"

# example of mesh adaptation with a sizing
remesh_options              = {}
remesh_options['gradation'] = 1.5
remesh_options['logfile'] = "remesh-adap-metric.log"

print " remeshing options : "
print remesh_options
print "\n remeshing done with : msh3d_iso           = pyamg.adapt_mesh(msh3d_iso, remesh_options) \n"

msh3d.pop('sensor',None)
msh3d['metric']  = create_sizing(msh3d)
print  "   Adaptation : iteration 1"
msh3d_iso        = pyamg.adapt_mesh(msh3d, remesh_options)

for ite in range(2,5):
  print  "   Adaptation : iteration %d " %(ite)    
  msh3d_iso['metric'] = create_sizing(msh3d_iso)
  msh3d_iso           = pyamg.adapt_mesh(msh3d_iso, remesh_options)

print "\n output : cube_iso.meshb \n"  
pyamg.write_mesh(msh3d_iso,"cube_iso.meshb")  

