import simncad.ncad as ncw
import time
from simphony.cuds.particles import Particles
from simphony.core.cuba import CUBA
from simncad.auxiliar.ncad_types import SHAPE_TYPE, AXIS_TYPE
from simncad.auxiliar.celldata_parser import read_cd


nc = ncw.nCad(project='SiliconDioxideNanosphere')

sio2 = read_cd('../cd/sio2.cd')

sio2 = nc.add_dataset(sio2)
# ncad.show('sio2')

sphere = Particles('sio2-sphere')

data = sphere.data
data[CUBA.NAME_UC] = "sio2"
data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
data[CUBA.SHAPE_CENTER] = (10,0,0)
data[CUBA.SHAPE_RADIUS] = 25.0
# data[CUBA.CRYSTAL_ORIENTATION_1] = ((1,0,0), (0,0,1))
# data[CUBA.SHAPE_ORIENTATION_1] = (AXIS_TYPE.X, (0,1,0))
sphere.data = data

sphere = nc.add_dataset(sphere)

t_init = time.clock()
assembly = nc.run()
t_final = time.clock()

atoms_count = 0
bonds_count = 0
for part in sphere.iter_particles():
    atoms_count += 1

for bond in sphere.iter_bonds():
    bonds_count += 1

print "Elapsed time: ", t_final-t_init
print "N. atoms:     ", atoms_count
print "N. bonds:     ", bonds_count


nc.show('sio2-sphere')


