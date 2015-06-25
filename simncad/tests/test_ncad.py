"""
    Testing for particlesclasses module.
"""

import unittest
import uuid
import random

import simncad.ncad as ncw
from simphony.cuds.particles import Particle, Bond, Particles
from simphony.core.data_container import DataContainer
from simphony.core.cuba import CUBA
from simncad.auxiliar.ncad_types import SHAPE_TYPE, SYMMETRY_GROUP, AXIS_TYPE


class NCadWrapperTestCase(unittest.TestCase):
    def setUp(self):
        self.ncad = ncw.nCad('test_ncad' + str(random.random()))
        
    def test_add_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        returned_cell = self.ncad.get_particle_container(cell_name) 
        self.assertIsNotNone(returned_cell)
        self.assertEqual(returned_cell.name, cell_name)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        # optional:
        # data[CUBA_EXT.CRYSTAL_ORIENTATION_1] = ((1,2,3),(1,0,1))
        # data[CUBA_EXT.CRYSTAL_ORIENTATION_2] = ((1,2,5),(1,1,1))
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        # optional:
        # data[CUBA_EXT.SHAPE_ORIENTATION_1] = (AXIS_TYPE.X,(0,1,0))
        # data[CUBA_EXT.SHAPE_ORIENTATION_2] = (AXIS_TYPE.Y,(0,1,1))
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        returned_component = self.ncad.get_particle_container(component_name)
        self.assertIsNotNone(returned_component)
        self.assertEqual(returned_component.name, component_name)
        
    def test_exception_when_adding_duplicated_pc(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        with self.assertRaises(Exception):
            ncad_cell = self.ncad.add_particle_container(cell, options)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        with self.assertRaises(Exception):
            ncad_component = self.ncad.add_particle_container(component, options)
        
    def test_exception_when_adding_incompleted_pc(self):
        # cell
        # we omit lattice parameters
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        options = {}
        options['type'] = 'cell'
        # COMMENTED FTM AS WE DONT HAVE ANY MANDATORY CELL ATTRIBUTE
        # with self.assertRaises(Exception):
            # self.ncad.add_particle_container(cell, options)
        # we omit type
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        with self.assertRaises(Exception):
            self.ncad.add_particle_container(cell, options)
        # component
        # we omit cell name
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        with self.assertRaises(Exception):
            self.ncad.add_particle_container(component, options)

    def test_delete_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        self.ncad.delete_particle_container(cell_name)
        with self.assertRaises(Exception):
            returned_cell = self.ncad.get_particle_container(cell_name)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        self.ncad.delete_particle_container(component_name)
        with self.assertRaises(Exception):
            returned_component = self.ncad.get_particle_container(component_name)

    def test_exception_when_deleting_unexsinting_pc(self):
        with self.assertRaises(Exception):
            self.ncad.delete_particle_container('foo')

    def test_iter_all_particle_containers(self):
        names = []
        cell_name = 'cell_pc' + str(random.random())
        names.append(cell_name)
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        component_name = 'component_pc' + str(random.random())
        names.append(component_name)
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        for pc in self.ncad.iter_particle_containers():
            self.assertIn(pc.name, names)

    def test_iter_some_particle_containers(self):
        names = []
        cell_name = 'cell_pc' + str(random.random())
        names.append(cell_name)
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        component_name = 'component_pc' + str(random.random())
        names.append(component_name)
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        for pc in self.ncad.iter_particle_containers(names):
            self.assertIn(pc.name, names)
        
    def test_iter_unexisting_particle_container(self):
        names = ['foo', 'bar']
        with self.assertRaises(Exception):
            for pc in self.ncad.iter_particle_containers(names):
                print pc.name

    def test_run(self):
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_id1 = ncad_cell.add_particle(particle)
        particle = Particle((0.25,0.25,0.25))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C2'
        particle_id2 = ncad_cell.add_particle(particle)
        bond = Bond((particle_id1, particle_id2))
        ncad_cell.add_bond(bond)
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_BLOCK_UC
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_LENGTH_UC] = (2,2,2)
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        assembly = self.ncad.run()
        count = 0
        for part in assembly.iter_particles():
            count += 1
        self.assertEqual(count, 16)
        count = 0
        for bond in assembly.iter_bonds():
            count += 1
        self.assertEqual(count, 8)
        
    def test_update_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell = self.ncad.add_particle_container(cell, options)
        cur_data = ncad_cell.get_data()
        # cur_data[CUBA_EXT.LATTICE_UC_ABC] = (3,3,3)
        # cur_data[CUBA_EXT.LATTICE_UC_ANGLES] = (45,60,120)
        # cur_data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P213
        ncad_cell.set_data(cur_data)
        cur_data = ncad_cell.get_data()
        # self.assertItemsEqual(cur_data[CUBA_EXT.LATTICE_UC_ABC], (3,3,3))
        # self.assertItemsEqual(cur_data[CUBA_EXT.LATTICE_UC_ANGLES], (45,60,120))
        # self.assertEqual(cur_data[CUBA_EXT.SYMMETRY_GROUP], SYMMETRY_GROUP.P213)
        
        cell_name_replace = 'cell_pc_replace'
        cell = Particles(name=cell_name_replace)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        options = {}
        options['type'] = 'cell'
        ncad_cell_replace = self.ncad.add_particle_container(cell, options)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        # data[CUBA_EXT.CRYSTAL_ORIENTATION_1] = ((1,1,1),(0,1,0))
        # data[CUBA_EXT.CRYSTAL_ORIENTATION_2] = ((1,0,0),(0,0,1))
        # data[CUBA_EXT.SHAPE_ORIENTATION_1] = (AXIS_TYPE.X,(0,1,0))
        # data[CUBA_EXT.SHAPE_ORIENTATION_2] = (AXIS_TYPE.Y,(0,0,1))
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        options['type'] = 'component'
        ncad_component = self.ncad.add_particle_container(component, options)
        cur_data = ncad_component.get_data()
        cur_data[CUBA.NAME_UC] = cell_name_replace
        cur_data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_CYLINDER
        # cur_data[CUBA_EXT.CRYSTAL_ORIENTATION_1] = ((2,2,2),(1,0,1))
        # cur_data[CUBA_EXT.CRYSTAL_ORIENTATION_2] = ((0,1,1),(1,1,0))
        # cur_data[CUBA_EXT.SHAPE_ORIENTATION_1] = (AXIS_TYPE.Z,(1,0,1))
        # cur_data[CUBA_EXT.SHAPE_ORIENTATION_2] = (AXIS_TYPE.X,(1,1,0))
        cur_data[CUBA.SHAPE_CENTER] = (10,20,30)
        cur_data[CUBA.SHAPE_RADIUS] = 10.0
        cur_data[CUBA.SHAPE_LENGTH] = (20,0,0)
        ncad_component.set_data(cur_data)
        cur_data = ncad_component.get_data()
        self.assertEqual(cur_data[CUBA.MATERIAL_TYPE], SHAPE_TYPE.DIM_3D_CYLINDER)
        self.assertEqual(cur_data[CUBA.SHAPE_CENTER], (10,20,30))
        self.assertEqual(cur_data[CUBA.SHAPE_RADIUS], 10.0)
        self.assertEqual(cur_data[CUBA.SHAPE_LENGTH], (20,0,0))
        # self.assertEqual(cur_data[CUBA_EXT.CRYSTAL_ORIENTATION_1], ((2,2,2),(1,0,1)))
        # self.assertEqual(cur_data[CUBA_EXT.CRYSTAL_ORIENTATION_2], ((0,1,1),(1,1,0)))
        # self.assertEqual(cur_data[CUBA_EXT.SHAPE_ORIENTATION_1], (AXIS_TYPE.Z,(1,0,1)))
        # self.assertEqual(cur_data[CUBA_EXT.SHAPE_ORIENTATION_2], (AXIS_TYPE.X,(1,1,0)))


class NCadParticlesTestCase1(unittest.TestCase):
    def setUp(self):
        ncad_temp = ncw.nCad('test_ncad' + str(random.random()))
        self.ncad = ncad_temp
        options = {}
        options['type'] = 'cell'
        self.cell_name = 'cell_pc' + str(random.random())
        self.component_name = 'component_pc' + str(random.random())
        self.cell = Particles(name=self.cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        self.cell.data = data
        self.cell = self.ncad.add_particle_container(self.cell, options)
        options['type'] = 'component'
        self.component = Particles(name=self.component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = self.cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        self.component.data = data
        self.component=self.ncad.add_particle_container(self.component, options)
        
    def test_add_particle(self):
        # cell
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_id = self.cell.add_particle(particle)
        self.assertTrue(self.cell.has_particle(particle_id))
        particle = Particle((0,0,0), uuid.UUID(int=99))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_id = self.cell.add_particle(particle)
        self.assertTrue(self.cell.has_particle(uuid.UUID(int=99)))
        # component
        particle = Particle((10,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'B'
        particle.data[CUBA.LABEL] = 'B3'
        particle_id = self.component.add_particle(particle)
        self.assertTrue(self.component.has_particle(particle_id))
        particle = Particle((10,10,10), uuid.UUID(int=99))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'S'
        particle.data[CUBA.LABEL] = 'S4'
        particle_id = self.component.add_particle(particle)
        self.assertTrue(self.component.has_particle(particle_id), 'particle_id: {}'.format(particle.uid))
        
    def test_exception_when_adding_duplicated_particle(self):
        # cell
        particle = Particle((10,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particle(particle)
        with self.assertRaises(Exception):
            self.cell.add_particle(particle)
        # component
        particle = Particle((10,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particle(particle)
        with self.assertRaises(Exception):
            self.component.add_particle(particle)

    def test_update_particle(self):
        # cell
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_id = self.cell.add_particle(particle)
        particle.coordinates = (1,1,1)
        self.cell.update_particle(particle)
        updated_particle = self.cell.get_particle(particle_id)
        self.assertEqual(updated_particle.coordinates, (1,1,1))
        # component
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_id = self.component.add_particle(particle)
        particle.coordinates = (1,1,1)
        self.component.update_particle(particle)
        updated_particle = self.component.get_particle(particle_id)
        self.assertEqual(updated_particle.coordinates, (1,1,1))
        
    def test_update_unexisting_particle(self):
        # cell
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particle(particle)
        fake_id = uuid.uuid4()
        particle.uid = fake_id
        with self.assertRaises(Exception):
            self.cell.update_particle(particle)
        # component
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particle(particle)
        fake_id = uuid.uuid4()
        particle.uid = fake_id
        with self.assertRaises(Exception):
            self.component.update_particle(particle)
        
    def test_remove_particle(self):
        # cell
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_id = self.cell.add_particle(particle)
        self.cell.remove_particle(particle_id)
        self.assertFalse(self.cell.has_particle(particle_id))
        # component
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_id = self.component.add_particle(particle)
        self.component.remove_particle(particle_id)
        self.assertFalse(self.component.has_particle(particle_id))
        
    def test_remove_unexisting_particle(self):
        # cell
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particle(particle)
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.cell.remove_particle(fake_id)
        # component
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particle(particle)
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.component.remove_particle(fake_id)
            
    def test_iter_some_particles(self):
        # cell
        ids = []
        ids_to_iterate = []
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            ids.append(self.cell.add_particle(particle))
        ids_to_iterate = ids[::2]
        iterated_ids = []
        for part in self.cell.iter_particles(ids_to_iterate):
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)
        # component
        ids = []
        ids_to_iterate = []
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            ids.append(self.component.add_particle(particle))
        ids_to_iterate = ids[::2]
        iterated_ids = []
        for part in self.component.iter_particles(ids_to_iterate):
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)

    def test_iter_all_particles(self):
        # cell
        ids = []
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            ids.append(self.cell.add_particle(particle))
        iterated_ids = []
        for part in self.cell.iter_particles():
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids, iterated_ids)
        # component
        ids = []
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            ids.append(self.component.add_particle(particle))
        iterated_ids = []
        for part in self.component.iter_particles():
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids, iterated_ids)
        
    def test_iter_unexisting_particles(self):
        # cell
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            self.cell.add_particle(particle)
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for part in self.cell.iter_particles(ids_to_iterate):
                iterated_ids.append(part.uid)
        # component
        for i in xrange(10):
            particle = Particle((i,i,i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            self.cell.add_particle(particle)
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for part in self.cell.iter_particles(ids_to_iterate):
                iterated_ids.append(part.uid)


class NCadParticlesTestCase2(unittest.TestCase):
    def setUp(self):
        ncad_temp = ncw.nCad('test_ncad' + str(random.random()))
        self.ncad = ncad_temp
        options = {}
        options['type'] = 'cell'
        self.cell_name = 'cell_pc' + str(random.random())
        self.component_name = 'component_pc' + str(random.random())
        self.cell = Particles(name=self.cell_name)
        data = DataContainer()
        # data[CUBA_EXT.LATTICE_UC_ABC] = (4,5,6)
        # data[CUBA_EXT.LATTICE_UC_ANGLES] = (90,90,90)
        # data[CUBA_EXT.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        self.cell.data = data
        self.cell = self.ncad.add_particle_container(self.cell, options)
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.particle_id_c1 = self.cell.add_particle(particle)
        particle = Particle((1,1,1))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O1'
        self.particle_id_o1 = self.cell.add_particle(particle)
        # ------------------------------------------------------------
        options['type'] = 'component'
        self.component = Particles(name=self.component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = self.cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0,0,0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        self.component.data = data
        self.component=self.ncad.add_particle_container(self.component, options)
        particle = Particle((0,0,0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'B'
        particle.data[CUBA.LABEL] = 'B1'
        self.particle_id_b1 = self.component.add_particle(particle)
        particle = Particle((1,1,1))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'S'
        particle.data[CUBA.LABEL] = 'S1'
        self.particle_id_s1 = self.component.add_particle(particle)

    def test_add_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        self.assertTrue(self.cell.has_bond(bond_id))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        self.assertTrue(self.component.has_bond(bond_id))
        
    def test_exception_when_adding_duplicated_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        with self.assertRaises(Exception):
            self.cell.add_bond(bond)
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        with self.assertRaises(Exception):
           self.component.add_bond(bond)

    def test_update_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        bond.particles = (self.particle_id_o1, self.particle_id_c1)
        self.cell.update_bond(bond)
        updated_bond = self.cell.get_bond(bond_id)
        self.assertEqual(bond.particles, (self.particle_id_o1, self.particle_id_c1))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        bond.particles = (self.particle_id_s1, self.particle_id_b1)
        self.component.update_bond(bond)
        updated_bond = self.component.get_bond(bond_id)
        self.assertEqual(bond.particles, (self.particle_id_s1, self.particle_id_b1))
        
    def test_update_unexisting_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        fake_id = uuid.uuid4()
        bond.uid = fake_id
        with self.assertRaises(Exception):
            self.cell.update_bond(bond)
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        fake_id = uuid.uuid4()
        bond.uid = fake_id
        with self.assertRaises(Exception):
            self.component.update_bond(bond)
        
    def test_remove_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        self.cell.remove_bond(bond_id)
        self.assertFalse(self.cell.has_bond(bond_id))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        self.component.remove_bond(bond_id)
        self.assertFalse(self.component.has_bond(bond_id))
        
    def test_remove_unexisting_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_id = self.cell.add_bond(bond)
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.cell.remove_bond(fake_id)
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_id = self.component.add_bond(bond)
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.component.remove_bond(fake_id)
            
    def test_iter_some_bonds(self):
        # cell
        ids_to_iterate = []
        iterated_ids = []
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        ids_to_iterate.append(self.cell.add_bond(bond))
        for bond in self.cell.iter_bonds(ids_to_iterate):
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)
        # component
        ids_to_iterate = []
        iterated_ids = []
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        ids_to_iterate.append(self.component.add_bond(bond))
        for bond in self.component.iter_bonds(ids_to_iterate):
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)

    def test_iter_all_bonds(self):
        # cell
        ids = []
        iterated_ids = []
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        ids.append(self.cell.add_bond(bond))
        for bond in self.cell.iter_bonds():
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids, iterated_ids)
        # component
        ids = []
        iterated_ids = []
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        ids.append(self.component.add_bond(bond))
        for bond in self.component.iter_bonds():
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids, iterated_ids)
        
    def test_iter_unexisting_bonds(self):
        # cell
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for bond in self.cell.iter_bonds(ids_to_iterate):
                iterated_ids.append(bond.uid)
        # component
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for bond in self.component.iter_bonds(ids_to_iterate):
                iterated_ids.append(bond.uid)

if __name__ == '__main__':
    unittest.main()
    



























