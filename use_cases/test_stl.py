import simncad.ncad as ncw
import time
from simphony.cuds.particles import Particles
from simphony.core.cuba import CUBA
from simncad.auxiliar.ncad_types import SHAPE_TYPE, AXIS_TYPE
from simncad.auxiliar.celldata_parser import read_cd


nc = ncw.nCad(project='SiliconDioxideSTL')

sio2 = read_cd('../cd/sio2.cd')

sio2 = nc.add_dataset(sio2)
# ncad.show('sio2')

stl = Particles('sio2-stl')

data = stl.data
data[CUBA.NAME_UC] = "sio2"
data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_STL
data[CUBA.FILE_STL] = "C:\\Sgenia\\Projects\\simphony_git_repos\\nfluid\\modelo3.STL"
data[CUBA.STL_MODE] = 0
data[CUBA.STL_SCALING] = 1
data[CUBA.STL_PADDING] = (0,0,0,0,0,3)
stl.data = data

stl = nc.add_dataset(stl)
t_init = time.clock()
assembly = nc.run()
t_final = time.clock()
atoms_count = 0
bonds_count = 0
for part in stl.iter_particles():
    atoms_count += 1

for bond in stl.iter_bonds():
    bonds_count += 1

print "project: ", nc.get_project_name()
print "Elapsed time: ", t_final-t_init
print "N. atoms:     ", atoms_count
print "N. bonds:     ", bonds_count

# nc.show('sio2-stl')

