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
from simncad.auxiliar.ncad_types import SHAPE_TYPE, SYMMETRY_GROUP
from simphony.core.cuds_item import CUDSItem
import simphony.engine as engine_api
from simphony.engine import EngineInterface, create_wrapper
from simphony import CUDS


class NCadWrapperTestCase(unittest.TestCase):
    def setUp(self):
        self.ncad = ncw.nCad(project='test_ncad' + str(random.random()))

    def test_add_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        returned_cell = self.ncad.add_dataset(cell)
        self.ncad.get_dataset(cell_name)
        self.assertIsNotNone(returned_cell)
        self.assertEqual(returned_cell.name, cell_name)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        # optional:
        # data[CUBA.CRYSTAL_ORIENTATION_1] = ((1,2,3),(1,0,1))
        # data[CUBA.CRYSTAL_ORIENTATION_2] = ((1,2,5),(1,1,1))
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        # optional:
        # data[CUBA.SHAPE_ORIENTATION_1] = (AXIS_TYPE.X,(0,1,0))
        # data[CUBA.SHAPE_ORIENTATION_2] = (AXIS_TYPE.Y,(0,1,1))
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        self.ncad.add_dataset(component)
        returned_component = self.ncad.get_dataset(component_name)
        self.assertIsNotNone(returned_component)
        self.assertEqual(returned_component.name, component_name)

    def test_exception_when_adding_duplicated_pc(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        self.ncad.add_dataset(cell)
        with self.assertRaises(Exception):
            self.ncad.add_dataset(cell)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        self.ncad.add_dataset(component)
        with self.assertRaises(Exception):
            self.ncad.add_dataset(component)

    def test_exception_when_adding_incompleted_pc(self):
        # cell
        # we omit lattice parameters
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        # COMMENTED FTM AS WE DONT HAVE ANY MANDATORY CELL ATTRIBUTE
        with self.assertRaises(Exception):
            self.ncad.add_dataset(cell)
        # we omit type
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        cell.data = data
        with self.assertRaises(Exception):
            self.ncad.add_dataset(cell)
        # component
        # we omit cell name
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        with self.assertRaises(Exception):
            self.ncad.add_dataset(component)

    def test_delete_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        self.ncad.add_dataset(cell)
        self.ncad.remove_dataset(cell_name)
        with self.assertRaises(Exception):
            self.ncad.get_dataset(cell_name)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        self.ncad.add_dataset(cell)
        self.ncad.add_dataset(component)
        self.ncad.remove_dataset(component_name)
        with self.assertRaises(Exception):
            self.ncad.get_dataset(component_name)

    def test_exception_when_deleting_unexsinting_pc(self):
        with self.assertRaises(Exception):
            self.ncad.remove_dataset('foo')

    def test_iter_all_particle_containers(self):
        names = []
        cell_name = 'cell_pc' + str(random.random())
        names.append(cell_name)
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        self.ncad.add_dataset(cell)
        component_name = 'component_pc' + str(random.random())
        names.append(component_name)
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        self.ncad.add_dataset(component)
        for pc in self.ncad.iter_datasets():
            self.assertIn(pc.name, names)

    def test_iter_some_particle_containers(self):
        names = []
        cell_name = 'cell_pc' + str(random.random())
        names.append(cell_name)
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        self.ncad.add_dataset(cell)
        component_name = 'component_pc' + str(random.random())
        names.append(component_name)
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        self.ncad.add_dataset(component)
        for pc in self.ncad.iter_datasets(names):
            self.assertIn(pc.name, names)

    def test_iter_unexisting_particle_container(self):
        names = ['foo', 'bar']
        with self.assertRaises(Exception):
            for pc in self.ncad.iter_datasets(names):
                print pc.name

    def test_run(self):
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        ncad_cell = self.ncad.add_dataset(cell)
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_id1 = ncad_cell.add_particles([particle])[0]
        particle = Particle((0.25, 0.25, 0.25))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C2'
        particle_id2 = ncad_cell.add_particles([particle])[0]
        bond = Bond((particle_id1, particle_id2))
        ncad_cell.add_bonds([bond])
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_BLOCK_UC
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_LENGTH_UC] = (2, 2, 2)
        component.data = data
        self.ncad.add_dataset(component)
        assembly = self.ncad.run()
        count = 0
        for part in assembly.iter_particles():
            count += 1
        self.assertEqual(count, 16)
        count = 0
        for bond in assembly.iter_bonds():
            count += 1

    def test_update_particle_container(self):
        # cell
        cell_name = 'cell_pc' + str(random.random())
        cell = Particles(name=cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        ncad_cell = self.ncad.add_dataset(cell)
        cur_data = ncad_cell.get_data()
        cur_data[CUBA.LATTICE_UC_ABC] = (3,3,3)
        cur_data[CUBA.LATTICE_UC_ANGLES] = (45,60,120)
        cur_data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P213
        ncad_cell.set_data(cur_data)
        cur_data = ncad_cell.get_data()
        self.assertItemsEqual(cur_data[CUBA.LATTICE_UC_ABC], (3,3,3))
        self.assertItemsEqual(cur_data[CUBA.LATTICE_UC_ANGLES],
                                   (45,60,120))
        self.assertEqual(cur_data[CUBA.SYMMETRY_GROUP],
                           SYMMETRY_GROUP.P213)

        cell_name_replace = 'cell_pc_replace'
        cell = Particles(name=cell_name_replace)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        cell.data = data
        self.ncad.add_dataset(cell)
        # component
        component_name = 'component_pc' + str(random.random())
        component = Particles(name=component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = cell_name
        # data[CUBA.CRYSTAL_ORIENTATION_1] = ((1,1,1),(0,1,0))
        # data[CUBA.CRYSTAL_ORIENTATION_2] = ((1,0,0),(0,0,1))
        # data[CUBA.SHAPE_ORIENTATION_1] = (AXIS_TYPE.X,(0,1,0))
        # data[CUBA.SHAPE_ORIENTATION_2] = (AXIS_TYPE.Y,(0,0,1))
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        component.data = data
        ncad_component = self.ncad.add_dataset(component)
        cur_data = ncad_component.get_data()
        cur_data[CUBA.NAME_UC] = cell_name_replace
        cur_data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_CYLINDER
        # cur_data[CUBA.CRYSTAL_ORIENTATION_1] = ((2,2,2),(1,0,1))
        # cur_data[CUBA.CRYSTAL_ORIENTATION_2] = ((0,1,1),(1,1,0))
        # cur_data[CUBA.SHAPE_ORIENTATION_1] = (AXIS_TYPE.Z,(1,0,1))
        # cur_data[CUBA.SHAPE_ORIENTATION_2] = (AXIS_TYPE.X,(1,1,0))
        cur_data[CUBA.SHAPE_CENTER] = (10, 20, 30)
        cur_data[CUBA.SHAPE_RADIUS] = 10.0
        cur_data[CUBA.SHAPE_LENGTH] = (20, 0, 0)
        ncad_component.set_data(cur_data)
        cur_data = ncad_component.get_data()
        self.assertEqual(cur_data[CUBA.MATERIAL_TYPE],
                         SHAPE_TYPE.DIM_3D_CYLINDER)
        self.assertEqual(cur_data[CUBA.SHAPE_CENTER], (10, 20, 30))
        self.assertEqual(cur_data[CUBA.SHAPE_RADIUS], 10.0)
        self.assertEqual(cur_data[CUBA.SHAPE_LENGTH], (20, 0, 0))
        # self.assertEqual(cur_data[CUBA.CRYSTAL_ORIENTATION_1],
        #                    ((2,2,2),(1,0,1)))
        # self.assertEqual(cur_data[CUBA.CRYSTAL_ORIENTATION_2],
        #                    ((0,1,1),(1,1,0)))
        # self.assertEqual(cur_data[CUBA.SHAPE_ORIENTATION_1],
        #                    (AXIS_TYPE.Z,(1,0,1)))
        # self.assertEqual(cur_data[CUBA.SHAPE_ORIENTATION_2],
        #                    (AXIS_TYPE.X,(1,1,0)))


class NCadParticlesTestCase1(unittest.TestCase):
    def setUp(self):
        ncad_temp = ncw.nCad(project='test_ncad' + str(random.random()))
        self.ncad = ncad_temp
        self.cell_name = 'cell_pc' + str(random.random())
        self.component_name = 'component_pc' + str(random.random())
        self.cell = Particles(name=self.cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        self.cell.data = data
        self.cell = self.ncad.add_dataset(self.cell)
        self.component = Particles(name=self.component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = self.cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        self.component.data = data
        self.component = self.ncad.add_dataset(self.component)

    def test_add_particle(self):
        # cell
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_ids = self.cell.add_particles([particle])
        for particle_id in particle_ids:
            self.assertTrue(self.cell.has_particle(particle_id))
        particle = Particle((0, 0, 0), uuid.UUID(int=99))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_id = self.cell.add_particles([particle])
        self.assertTrue(self.cell.has_particle(uuid.UUID(int=99)))
        # component
        particle = Particle((10, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'B'
        particle.data[CUBA.LABEL] = 'B3'
        particle_ids = self.component.add_particles([particle])
        for particle_id in particle_ids:
            self.assertTrue(self.component.has_particle(particle_id))
        particle = Particle((10, 10, 10), uuid.UUID(int=99))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'S'
        particle.data[CUBA.LABEL] = 'S4'
        particle_ids = self.component.add_particles([particle])
        for particle_id in particle_ids:
            self.assertTrue(self.component.has_particle(particle_id),
                            'particle_id: {}'.format(particle.uid))

    def test_exception_when_adding_duplicated_particle(self):
        # cell
        particle = Particle((10, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particles([particle])
        with self.assertRaises(Exception):
            self.cell.add_particles([particle])
        # component
        particle = Particle((10, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particles([particle])
        with self.assertRaises(Exception):
            self.component.add_particles([particle])

    def test_update_particle(self):
        # cell
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_ids = self.cell.add_particles([particle])
        particle.coordinates = (1, 1, 1)
        self.cell.update_particles([particle])
        for particle_id in particle_ids:
            updated_particle = self.cell.get_particle(particle_id)
            self.assertEqual(updated_particle.coordinates, (1, 1, 1))
        # component
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_ids = self.component.add_particles([particle])
        particle.coordinates = (1, 1, 1)
        self.component.update_particles([particle])
        for particle_id in particle_ids:
            updated_particle = self.component.get_particle(particle_id)
            self.assertEqual(updated_particle.coordinates, (1, 1, 1))

    def test_update_unexisting_particle(self):
        # cell
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particles([particle])
        fake_id = uuid.uuid4()
        particle.uid = fake_id
        with self.assertRaises(Exception):
            self.cell.update_particles([particle])
        # component
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particles([particle])
        fake_id = uuid.uuid4()
        particle.uid = fake_id
        with self.assertRaises(Exception):
            self.component.update_particles([particle])

    def test_remove_particle(self):
        # cell
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        particle_ids = self.cell.add_particles([particle])
        self.cell.remove_particles(particle_ids)
        for particle_id in particle_ids:
            self.assertFalse(self.cell.has_particle(particle_id))
        # component
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        particle_ids = self.component.add_particles([particle])
        self.component.remove_particles(particle_ids)
        for particle_id in particle_ids:
            self.assertFalse(self.component.has_particle(particle_id))

    def test_remove_unexisting_particle(self):
        # cell
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.cell.add_particles([particle])
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.cell.remove_particles([fake_id])
        # component
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O2'
        self.component.add_particles([particle])
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.component.remove_particles([fake_id])

    def test_iter_some_particles(self):
        # cell
        ids = []
        ids_to_iterate = []
        particles = []
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            particles.append(particle)
        ids = self.cell.add_particles(particles)
        ids_to_iterate = ids[::2]
        iterated_ids = []
        for part in self.cell.iter_particles(ids_to_iterate):
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)
        # component
        ids = []
        ids_to_iterate = []
        particles = []
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            particles.append(particle)
        ids = self.component.add_particles(particles)
        ids_to_iterate = ids[::2]
        iterated_ids = []
        for part in self.component.iter_particles(ids_to_iterate):
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)

    def test_iter_all_particles(self):
        # cell
        ids = []
        particles = []
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            particles.append(particle)
        ids = self.cell.add_particles(particles)
        iterated_ids = []
        for part in self.cell.iter_particles():
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids, iterated_ids)
        # component
        ids = []
        particles = []
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            particles.append(particle)
        ids = self.component.add_particles(particles)
        iterated_ids = []
        for part in self.component.iter_particles():
            iterated_ids.append(part.uid)
        self.assertItemsEqual(ids, iterated_ids)

    def test_iter_unexisting_particles(self):
        # cell
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            self.cell.add_particles([particle])
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for part in self.cell.iter_particles(ids_to_iterate):
                iterated_ids.append(part.uid)
        # component
        for i in xrange(10):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
            particle.data[CUBA.LABEL] = 'O' + str(i)
            self.cell.add_particles([particle])
        ids_to_iterate = [uuid.uuid4()]
        iterated_ids = []
        with self.assertRaises(Exception):
            for part in self.cell.iter_particles(ids_to_iterate):
                iterated_ids.append(part.uid)

    def test_count_of_particles(self):
        # cell
        n = 13
        for i in xrange(n):
            particle = Particle((i, i, i))
            particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
            particle.data[CUBA.LABEL] = 'C' + str(i)
            self.cell.add_particles([particle])
        self.assertEqual(self.cell.count_of(CUDSItem.PARTICLE), n)


class NCadParticlesTestCase2(unittest.TestCase):
    def setUp(self):
        ncad_temp = ncw.nCad(project='test_ncad' + str(random.random()))
        self.ncad = ncad_temp
        self.cell_name = 'cell_pc' + str(random.random())
        self.component_name = 'component_pc' + str(random.random())
        self.cell = Particles(name=self.cell_name)
        data = DataContainer()
        data[CUBA.LATTICE_UC_ABC] = (4,5,6)
        data[CUBA.LATTICE_UC_ANGLES] = (90,90,90)
        data[CUBA.SYMMETRY_GROUP] = SYMMETRY_GROUP.P1
        self.cell.data = data
        self.cell = self.ncad.add_dataset(self.cell)
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'C'
        particle.data[CUBA.LABEL] = 'C1'
        self.particle_id_c1 = self.cell.add_particles([particle])[0]
        particle = Particle((1, 1, 1))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'O'
        particle.data[CUBA.LABEL] = 'O1'
        self.particle_id_o1 = self.cell.add_particles([particle])[0]
        # ------------------------------------------------------------
        self.component = Particles(name=self.component_name)
        data = DataContainer()
        data[CUBA.NAME_UC] = self.cell_name
        data[CUBA.MATERIAL_TYPE] = SHAPE_TYPE.DIM_3D_SPHERE
        data[CUBA.SHAPE_CENTER] = (0, 0, 0)
        data[CUBA.SHAPE_RADIUS] = 5.0
        self.component.data = data
        self.component = self.ncad.add_dataset(self.component)
        particle = Particle((0, 0, 0))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'B'
        particle.data[CUBA.LABEL] = 'B1'
        self.particle_id_b1 = self.component.add_particles([particle])[0]
        particle = Particle((1, 1, 1))
        particle.data[CUBA.CHEMICAL_SPECIE] = 'S'
        particle.data[CUBA.LABEL] = 'S1'
        self.particle_id_s1 = self.component.add_particles([particle])[0]

    def test_add_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_ids = self.cell.add_bonds([bond])
        for bond_id in bond_ids:
            self.assertTrue(self.cell.has_bond(bond_id))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_ids = self.component.add_bonds([bond])
        for bond_id in bond_ids:
            self.assertTrue(self.component.has_bond(bond_id))

    def test_exception_when_adding_duplicated_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        self.cell.add_bonds([bond])
        with self.assertRaises(Exception):
            self.cell.add_bonds([bond])
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        self.component.add_bonds([bond])
        with self.assertRaises(Exception):
            self.component.add_bonds([bond])

    def test_update_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_ids = self.cell.add_bonds([bond])
        bond.particles = (self.particle_id_o1, self.particle_id_c1)
        self.cell.update_bonds([bond])
        bond_id = bond_ids[0]
        self.cell.get_bond(bond_id)
        self.assertEqual(bond.particles, (self.particle_id_o1,
                                          self.particle_id_c1))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_ids = self.component.add_bonds([bond])
        bond.particles = (self.particle_id_s1, self.particle_id_b1)
        self.component.update_bonds([bond])
        bond_id = bond_ids[0]
        self.component.get_bond(bond_id)
        self.assertEqual(bond.particles, (self.particle_id_s1,
                                          self.particle_id_b1))

    def test_update_unexisting_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        self.cell.add_bonds([bond])
        fake_id = uuid.uuid4()
        bond.uid = fake_id
        with self.assertRaises(Exception):
            self.cell.update_bonds([bond])
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        self.component.add_bonds([bond])
        fake_id = uuid.uuid4()
        bond.uid = fake_id
        with self.assertRaises(Exception):
            self.component.update_bonds([bond])

    def test_remove_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        bond_ids = self.cell.add_bonds([bond])
        self.cell.remove_bonds(bond_ids)
        for bond_id in bond_ids:
            self.assertFalse(self.cell.has_bond(bond_id))
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        bond_ids = self.component.add_bonds([bond])
        self.component.remove_bonds(bond_ids)
        for bond_id in bond_ids:
            self.assertFalse(self.component.has_bond(bond_id))

    def test_remove_unexisting_bond(self):
        # cell
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        self.cell.add_bonds([bond])
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.cell.remove_bonds([fake_id])
        # component
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        self.component.add_bonds([bond])
        fake_id = uuid.uuid4()
        with self.assertRaises(Exception):
            self.component.remove_bonds([fake_id])

    def test_iter_some_bonds(self):
        # cell
        ids_to_iterate = []
        iterated_ids = []
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        ids_to_iterate = self.cell.add_bonds([bond])
        for bond in self.cell.iter_bonds(ids_to_iterate):
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)
        # component
        ids_to_iterate = []
        iterated_ids = []
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        ids_to_iterate = self.component.add_bonds([bond])
        for bond in self.component.iter_bonds(ids_to_iterate):
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids_to_iterate, iterated_ids)

    def test_iter_all_bonds(self):
        # cell
        ids = []
        iterated_ids = []
        bond = Bond((self.particle_id_c1, self.particle_id_o1))
        ids = self.cell.add_bonds([bond])
        for bond in self.cell.iter_bonds():
            iterated_ids.append(bond.uid)
        self.assertItemsEqual(ids, iterated_ids)
        # component
        ids = []
        iterated_ids = []
        bond = Bond((self.particle_id_b1, self.particle_id_s1))
        ids = self.component.add_bonds([bond])
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

    def test_count_of_bonds(self):
        # cell
        n = 17
        for i in xrange(n):
            bond = Bond((self.particle_id_c1, self.particle_id_o1))
            self.cell.add_bonds([bond])
        self.assertEqual(self.cell.count_of(CUDSItem.BOND), n)


if __name__ == '__main__':
    unittest.main()
