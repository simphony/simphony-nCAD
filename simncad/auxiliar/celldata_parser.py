import sys
import uuid
import symmetry

from simphony.cuds.particles import Particles, Particle, Bond
from simphony.core.data_container import DataContainer
from simphony.core.cuba import CUBA

def read_cd(file_name):
    f = open(file_name, "r")
    
    # !! We need to get the proper name !! - extract from file_name
    # name = f.readline().strip()
    # name_frags = name.split('/')
    # #print name
    # #print name_frags[-1]

    # cell = Particles(name_frags[-1])
    
    name = file_name.split('/')[-1]
    name = name.split('.')[0]
    #print name
    cell = Particles(name)
    
    data = cell.data

    # where to put this dictionary for atoms
    # ID way
    datoms_id = dict()
    # label way
    datoms_label = dict()

    #??	data[CUBA.NAME] = name
    #??	data[CUBA.NAME_UC] = name

    tmp = f.readline()
    tmp = f.readline()
    tmp = f.readline()
    symmetry_name = f.readline()
    #print symmetry_name
    symmetry_number = symmetry.GetSymmetryNumberByHM(symmetry_name)
    if symmetry_number == -1:
    	symmetry_number = 1 #??? default value???

    # COMMENTED FTM AS WE DONT HAVE SYMMETRY_GROUP OFFICIALY IN THE CUBA LIST -
    # data[CUBA.SYMMETRY_GROUP] = symmetry_number
    # -------------------------------------------------------------------------
#OLD    data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
    tmp = f.readline()


    tmp = f.readline()
    alpha = float(tmp)
    #print alpha
    tmp = f.readline()
    betta = float(tmp)
    #print betta
    tmp = f.readline()
    gamma = float(tmp)
    #print gamma
    tmp = f.readline()
    a = float(tmp)
    #print a
    tmp = f.readline()
    b = float(tmp)
    #print b
    tmp = f.readline()
    c = float(tmp)
    #print c

    # """
    # COMMENTED FTM AS THESE ARE NOT OFFICIAL CUBA KEYWORDS -------------------
    # data[CUBA.LATTICE_UC_ABC] = (a, b, c)
    # data[CUBA.LATTICE_UC_ANGLES] = (alpha, betta, gamma)
    # -------------------------------------------------------------------------
    cell.data = data
    # """
    tmp = f.readline()
    NAtoms = int(tmp)
    #print "NAtoms", NAtoms

    #!!! there is no ID in CD file !!!
    ncad_id = 0
    for atom in xrange(NAtoms):
        tmp = f.readline()
        items = tmp.split()

        Label = items[0]
        #print Label
        Element = items[1]
        #print Element

        x = float(items[2])
        #print x
        y = float(items[3])
        #print y
        z = float(items[4])
        #print z
        occupancy = float(items[5])
        #print occupancy
        # """		
        particle = Particle((x, y, z))
        particle.data[CUBA.CHEMICAL_SPECIE] = Element
        particle.data[CUBA.LABEL] = Label
        particle.data[CUBA.OCCUPANCY] = occupancy
        particle_id = cell.add_particle(particle)
        # ID way
        # fake ncad_id !!!
        datoms_id[ncad_id] = particle_id
        ncad_id += 1
        # label way
        datoms_label[Label] = particle_id

        # """

    tmp = f.readline()
    NBonds = int(tmp)
    #print "NBonds", NBonds

    for bond in xrange(NBonds):
        tmp = f.readline()
        items = tmp.split()

        Label1 = items[0]
        #print Label1
        Label2 = items[1]
        #print Label2
        ID1 = int(items[2])
        #print ID1
        ID2 = int(items[3])
        #print ID2

        # """		
        # ID way
        bond = Bond((datoms_id[ID1], datoms_id[ID2]), uuid.uuid4())
        # label way
        bond = Bond((datoms_label[Label1], datoms_label[Label2]), uuid.uuid4())
        cell.add_bond(bond)
        # """

    return cell
    # return 0


# partical_container = read_cd("GaAs.cd")


