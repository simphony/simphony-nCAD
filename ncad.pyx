from libcpp.string cimport string
from libcpp.map cimport map
from cython.operator cimport dereference as deref, preincrement as inc

from simphony.core.data_container import DataContainer
import simphony.cuds.particles as p
from simphony.core.cuba import CUBA
cimport c_ncad

import copy
import uuid
from ncad_types import SHAPE_TYPE, SYMMETRY_GROUP, AXIS_TYPE


cdef class _NCadParticles:
    """Particle Container wrapper class for nCad adapter.

    This class is private and used as a proxy to the real data inside nCad.

    Attributes
    ----------
    thisptr : CNCadParticleContainer pointer
        pointer to the C++ particle container
    _data : DataContainer
        data attributes of the particle container

    """
    cdef c_ncad.CNCadParticleContainer *thisptr
    cdef public object _data

    def __init__(self, *args):
        """Python constructor."""
        self._data = DataContainer()

    def __cinit__(self, *args):
        """Cython constructor.

        Parameters
        ----------
        args[0] : str
            values: 'component' or 'cell', indicating the type of the pc.

        Raises
        ------
        Exception when there is no type argument.

        """
        type = args[0]
        if type == 'component':
            self.thisptr = new c_ncad.CNCadComponent()
        elif type == 'cell':
            self.thisptr = new c_ncad.CNCadCell()
        else:
            raise Exception("No type specified! ('component' or 'cell')")

    def __dealloc__(self):
        """Cython destructor."""
        del self.thisptr

    # Common ABC interface ====================================================
    # =========================================================================
    def add_particle(self, particle):
        """Adds the particle to the container.

        Parameters
        ----------
        particle : Particle
            the new particle in the container.

        Returns
        -------
        Id of the added particle.

        Raises
        -------
        Exception when the particle already exists in the container, or when
        the particle can't be added due to internal errors.
        """
        if particle.uid is not None and self.has_particle(particle.uid):
            raise Exception('Duplicated particle! {}'.format(particle.uid))
        cdef c_ncad.CParticleInfo part_info
        self._matchFromParticle(particle, part_info)
        self.thisptr.AddParticle(part_info)
        return particle.uid

    def add_bond(self, bond):
        """Adds the bond to the container.

        Parameters
        ----------
        bond : Bond
            the new bond in the container.

        Returns
        -------
        Id of the added bond.

        Raises
        -------
        Exception when the bond already exists in the container, or when
        the bond can't be added due to internal errors.
        """
        if bond.uid is not None and self.has_bond(bond.uid):
            raise Exception('Duplicated bond! {}'.format(bond.uid))
        cdef c_ncad.CBondInfo bond_info
        self._matchFromBond(bond, bond_info)
        self.thisptr.AddBond(bond_info)
        return bond.uid

    def update_particle(self, particle):
        """Updates the particle inside the container.

        Parameters
        ----------
        particle : Particle
            the particle to replace

        Raises
        ------
        Exception when the particle doesn't exists in the container.

        """
        cdef c_ncad.CParticleInfo part_info
        self._matchFromParticle(particle, part_info)
        self.thisptr.UpdateParticle(part_info)

    def update_bond(self, bond):
        """Updates the bond inside the container.

        Parameters
        ----------
        bond : Bond
            the bond to replace

        Raises
        ------
        Exception when the bond doesn't exists in the container.

        """
        cdef c_ncad.CBondInfo bond_info
        self._matchFromBond(bond, bond_info)
        self.thisptr.UpdateBond(bond_info)

    def get_particle(self, uid):
        """Returns a copy of the requested particle.

        Parameters
        ----------
        uid : uuid.UUID
            id of the requested particle.

        Returns
        -------
        A copy of the particle.

        Raises
        ------
        Exception when the requested particle doesn't exists in the container.

        """
        cdef c_ncad.CParticleInfo *part_info = NULL
        part_info = self.thisptr.GetParticleInfo(uid.hex)
        if part_info is not NULL:
            res = p.Particle((part_info.x, part_info.y, part_info.z),
                             uid)
            res.data[CUBA.LABEL] = part_info.label
            res.data[CUBA.CHEMICAL_SPECIE] = part_info.specie
            res.data[CUBA.OCCUPANCY] = part_info.occupancy
            c_ncad.delete_pointer(part_info)
            return res
        else:
            raise Exception("Particle {0} not found!".format(uid))

    def get_bond(self, uid):
        """Returns a copy of the requested bond.

        Parameters
        ----------
        uid : uuid.UUID
            id of the requested bond.

        Returns
        -------
        A copy of the bond.

        Raises
        ------
        Exception when the requested bond doesn't exists in the container.

        """
        cdef c_ncad.CBondInfo *bond_info = NULL
        bond_info = self.thisptr.GetBondInfo(uid.hex)
        if bond_info is not NULL:
            id1 = uuid.UUID(hex=bond_info.atom1)
            id2 = uuid.UUID(hex=bond_info.atom2)
            res = p.Bond((id1, id2), uid)
            c_ncad.delete_pointer(bond_info)
            return res
        else:
            raise Exception("Bond {0} not found!".format(uid))

    def remove_particle(self, uid):
        """Deletes the particle from the container.

        Parameters
        ----------
        uid : uuid.UUID
            id of the particle

        Raises
        ------
        Exception if the particle doesn't exists.

        """
        self.thisptr.RemoveParticle(uid.hex)

    def remove_bond(self, uid):
        """Deletes the bond from the container.

        Parameters
        ----------
        uid : uuid.UUID
            id of the bond

        Raises
        ------
        Exception if the bond doesn't exists.

        """
        self.thisptr.RemoveBond(uid.hex)

    def has_particle(self, id):
        """Indicates if the particle with the given id is in the container.

        Parameters
        ----------
        id : uuid.UUID
            id of the particle

        Returns
        -------
        True if the particle exists, false otherwise.

        """
        return self.thisptr.HasParticle(id.hex)

    def has_bond(self, id):
        """Indicates if the bond with the given id is in the container.

        Parameters
        ----------
        id : uuid.UUID
            id of the bond

        Returns
        -------
        True if the bond exists, false otherwise.

        """
        return self.thisptr.HasBond(id.hex)

    def iter_particles(self, ids=None):
        """Iterates over the given particles of the container; if parameter is
        omitted, it will iterate over all particles inside.

        Parameters
        ----------
        ids : iterable
            sequence with the ids to iterate.

        Raises
        ------
        Exception if any of the ids is not in the container.

        """
        if ids:
            try:
                return self._iter_some_particles(ids)
            except KeyError as exception:
                raise exception
        else:
            return self._iter_all_particles()

    def iter_bonds(self, ids=None):
        """Iterates over the given bonds of the container; if parameter is
        omitted, it will iterate over all bonds inside.

        Parameters
        ----------
        ids : iterable
            sequence with the ids to iterate.

        Raises
        ------
        Exception if any of the ids is not in the container.

        """
        if ids:
            try:
                return self._iter_some_bonds(ids)
            except KeyError as exception:
                raise exception
        else:
            return self._iter_all_bonds()

    # Additional methods ======================================================
    # =========================================================================
    def get_data(self):
        """Method to get a copy of the current state of the data attributes
        of the data container of the pc.

        Returns
        -------
        A copy of the data container.

        """
        return copy.deepcopy(self._data)

    def set_data(self, new_data):
        """Method to set the current state of the data attributes
        of the data container of the pc.

        Parameters
        ----------
        new_data : DataContainer
            the new data container with the new parameters.

        Raises
        ------
        Exception if any of the new parameters are missing or can't
        be processed.

        """
        cdef c_ncad.CParticleContainerInfo pc_info
        pc_info = c_ncad.CParticleContainerInfo()
        if CUBA.CRYSTAL_ORIENTATION_1 in new_data:
            c_rot = new_data[CUBA.CRYSTAL_ORIENTATION_1]
            pc_info.crystal_rotation11[0] = c_rot[0][0]
            pc_info.crystal_rotation11[1] = c_rot[0][1]
            pc_info.crystal_rotation11[2] = c_rot[0][2]
            pc_info.crystal_rotation12[0] = c_rot[1][0]
            pc_info.crystal_rotation12[1] = c_rot[1][1]
            pc_info.crystal_rotation12[2] = c_rot[1][2]
            if CUBA.CRYSTAL_ORIENTATION_2 in new_data:
                c_rot = new_data[CUBA.CRYSTAL_ORIENTATION_2]
                pc_info.crystal_rotation21[0] = c_rot[0][0]
                pc_info.crystal_rotation21[1] = c_rot[0][1]
                pc_info.crystal_rotation21[2] = c_rot[0][2]
                pc_info.crystal_rotation22[0] = c_rot[1][0]
                pc_info.crystal_rotation22[1] = c_rot[1][1]
                pc_info.crystal_rotation22[2] = c_rot[1][2]
        if CUBA.SHAPE_ORIENTATION_1 in new_data:
            s_rot = new_data[CUBA.SHAPE_ORIENTATION_1]
            pc_info.shape_info.rotation_axis1 = s_rot[0]
            pc_info.shape_info.shape_rotation1[0] = s_rot[1][0]
            pc_info.shape_info.shape_rotation1[1] = s_rot[1][1]
            pc_info.shape_info.shape_rotation1[2] = s_rot[1][2]
            if CUBA.SHAPE_ORIENTATION_2 in new_data:
                s_rot = new_data[CUBA.SHAPE_ORIENTATION_2]
                pc_info.shape_info.rotation_axis2 = s_rot[0]
                pc_info.shape_info.shape_rotation2[0] = s_rot[1][0]
                pc_info.shape_info.shape_rotation2[1] = s_rot[1][1]
                pc_info.shape_info.shape_rotation2[2] = s_rot[1][2]
        if CUBA.LATTICE_UC_ABC in new_data:
            abc = new_data[CUBA.LATTICE_UC_ABC]
            pc_info.a = abc[0]
            pc_info.b = abc[1]
            pc_info.c = abc[2]
        if CUBA.LATTICE_UC_ANGLES in new_data:
            angles = new_data[CUBA.LATTICE_UC_ANGLES]
            pc_info.alpha = angles[0]
            pc_info.beta = angles[1]
            pc_info.gamma = angles[2]
        if CUBA.SYMMETRY_GROUP in new_data:
            pc_info.symmetry_group = new_data[CUBA.SYMMETRY_GROUP]
        if CUBA.SHAPE_TYPE in new_data:
            pc_info.shape_info.shape = new_data[CUBA.SHAPE_TYPE]
        if CUBA.SHAPE_CENTER in new_data:
            center = new_data[CUBA.SHAPE_CENTER]
            pc_info.shape_info.centerX = center[0]
            pc_info.shape_info.centerY = center[1]
            pc_info.shape_info.centerZ = center[2]
        if CUBA.SHAPE_LENGTH in new_data:
            lengthXYZ = new_data[CUBA.SHAPE_LENGTH]
            pc_info.shape_info.lengthX = lengthXYZ[0]
            if len(lengthXYZ) == 3:
                pc_info.shape_info.lengthY = lengthXYZ[1]
                pc_info.shape_info.lengthZ = lengthXYZ[2]
        if CUBA.SHAPE_LENGTH_UC in new_data:
            lengthXYZUC = new_data[CUBA.SHAPE_LENGTH_UC]
            pc_info.shape_info.lengthXUC = lengthXYZUC[0]
            pc_info.shape_info.lengthYUC = lengthXYZUC[1]
            pc_info.shape_info.lengthZUC = lengthXYZUC[2]
        if CUBA.SHAPE_RADIUS in new_data:
            pc_info.shape_info.radius = new_data[CUBA.SHAPE_RADIUS]
        if CUBA.SHAPE_SIDE in new_data:
            pc_info.shape_info.side = new_data[CUBA.SHAPE_SIDE]
        if CUBA.NAME_UC in new_data:
            pc_info.name_uc = new_data[CUBA.NAME_UC]
        self.thisptr.Update(pc_info)
        self._data = new_data

    # Private methods =========================================================
    # =========================================================================
    cdef _matchFromParticle(self, p_from, c_ncad.CParticleInfo & part_info):
        if p_from.uid is None:
            p_from.uid = uuid.uuid4()
        part_info.id = p_from.uid.hex
        part_info.x = p_from.coordinates[0]
        part_info.y = p_from.coordinates[1]
        part_info.z = p_from.coordinates[2]
        part_info.specie = p_from.data[CUBA.CHEMICAL_SPECIE]
        part_info.label = p_from.data[CUBA.LABEL]
        part_info.occupancy = 1

    cdef _matchToParticle(self, c_ncad.CParticleInfo & part_info, p_to):
        p_to.uid = uuid.UUID(hex=part_info.id)
        # p_to.uid = uuid.UUID(hex=id)
        p_to.coordinates[0] = part_info.x
        p_to.coordinates[1] = part_info.y
        p_to.coordinates[2] = part_info.z
        p_to.data = DataContainer()
        p_to.data[CUBA.CHEMICAL_SPECIE] = part_info.specie
        p_to.data[CUBA.LABEL] = part_info.label
        p_to.data[CUBA.OCCUPANCY] = part_info.occupancy

    cdef _matchFromBond(self, p_from, c_ncad.CBondInfo & bond_info):
        if p_from.uid is None:
            p_from.uid = uuid.uuid4()
        bond_info.id = p_from.uid.hex
        bond_info.atom1 = p_from.particles[0].hex
        bond_info.atom2 = p_from.particles[1].hex

    cdef _matchToBond(self, c_ncad.CBondInfo part_info, p_to):
        pass

    def _iter_some_particles(self, cur_ids):
        for cur_id in cur_ids:
            try:
                res = self.get_particle(cur_id)
                yield res
            except KeyError:
                raise KeyError('id {} not found!'.format(cur_id))

    def _iter_all_particles(self):
        for cur_element in self.thisptr.particles:
            cur_id = cur_element.first
            id = uuid.UUID(cur_id)
            res = self.get_particle(id)
            yield res

    def _iter_some_bonds(self, cur_ids):
        for cur_id in cur_ids:
            try:
                res = self.get_bond(cur_id)
                yield res
            except KeyError:
                raise KeyError('id {} not found!'.format(cur_id))

    def _iter_all_bonds(self):
        for cur_element in self.thisptr.bonds:
            cur_id = cur_element.first
            id = uuid.UUID(cur_id)
            res = self.get_bond(id)
            yield res

    def _get_name(self):
        return self.thisptr.name

    def _set_name(self, new_name):
        self.thisptr.name = new_name
    # =========================================================================
    # =========================================================================
    name = property(_get_name, _set_name)


cdef class nCad:
    """Wrapper class for nCad engine.

    Attributes
    ----------
    thisptr : c_ncad.CNCadSimphony pointer
       pointer to nCad C++ adapter.
    _components : dictionary
        dictionary of components inside nCad
    _cells : dictionary
        dictionary of cells inside nCad
    _session_name : str
        the name of the current started session / project
    CM : dictionary
        Computational method
    BC : dictionary
        Boundary conditions
    SP : dictionary
        System parameters
    Notes
    -----
    The session name is mandatory. For the moment, if the user specifies a name
    of a session that already exists, it will empty and overwrite that session.
    Only one instance of nCad at a time is allowed.
    """
    cdef c_ncad.CNCadSimphony *thisptr
    cdef object _components
    cdef object _cells
    cdef string _session_name
    # --------------------
    cdef object CM
    cdef object BC
    cdef object SP

    def __init__(self, *args):
        """Python constructor."""
        self._components = {}
        self._cells = {}

    def __cinit__(self, *args):
        """Cython constructor.

        Parameters
        ----------
        args[0] : str
            the session name (project name) which ncad will use to store
            the data in physycal disk.

        """
        self._session_name = args[0]
        if c_ncad.pCNCadSimphony is NULL:
            c_ncad.pCNCadSimphony = new c_ncad.CNCadSimphony()
        self.thisptr = c_ncad.pCNCadSimphony
        self.thisptr.LoadSession(self._session_name)


    def __dealloc__(self):
        """Cython destructor."""
        c_ncad.delete_pointer(c_ncad.pCNCadSimphony)
        del self.thisptr
        self.thisptr = NULL


    # Common ABC interface ====================================================
    # =========================================================================
    def run(self):
        """Run method of the nCad module.

        Using this, nCad will take all the particle containers of 'component'
        type inside the current session and process them in a single "assembly"
        particle container (of Simphony cuds classes) and return them.

        Returns
        -------
        A ParticleContainer of Simphony with the processed components.

        """
        self.thisptr.ProcessAll()
        res = p.Particles('__ASSEMBLY__')
        cdef c_ncad.CNCadParticleContainer *assembly
        assembly = new c_ncad.CNCadParticleContainer()
        assembly.name = res.name # we don't need this atm

        self.thisptr.BeginAssembly()

        self.thisptr.GetAssemblyAtoms(assembly)
        self._newAtomsFromAssembly(assembly, res)
        self.thisptr.GetAssemblyBonds(assembly)
        self._newBondsFromAssembly(assembly, res)

        self.thisptr.EndAssembly()

        del assembly
        return res

    def add_particle_container(self, pc, options=None):
        """Adds a new particle container to nCad. It will be treated as
        a cell or a component depending of the options['type'] parameter.

        Parameters
        ----------
        options : dictionary
            Dictionary containing some input parameters for nCad.

        Returns
        -------
        A _NCadParticles instance of the added particle container
        inside nCad.

        Raises
        ------
        KeyError if the type parameter inside options is missing or incorrect.
        Exception if a particle container with the same name has been added
        previously.

        """
        try:
            type = options['type']
        except KeyError:
            raise KeyError('Error: "type" parameter was not in "options" '
                           '(type="component" or type="cell")')
        if type is 'cell':
            if pc.name not in self._cells.keys():
                return self._add_cell(pc)
            else:
                raise Exception('Duplicated cell!')
        elif type is 'component':
            if pc.name not in self._components.keys():
                return self._add_component(pc, pc.data[CUBA.NAME_UC])
            else:
                raise Exception('Duplicated component!')
        else:
            raise Exception('Error: "type" parameter was incorrect '
                            '(type="component" or type="cell")')

    def get_particle_container(self, name):
        """Returns a _NCadParticles instance of the requested pc.

        Parameters
        ----------
        name : str
            name of the requested particle container.

        Returns
        -------
        The _NCadParticles instance of the pc.

        Raises
        ------
        KeyError if the particle container doesn't exists.

        """
        try:
            return self._get_component(name)
        except KeyError:
            try:
                return self._get_cell(name)
            except KeyError:
                raise KeyError('Particle Container "{0}" not found!'
                                .format(name))

    def delete_particle_container(self, name):
        """Deletes a particle container added to the current session of nCad.

        Parameters
        ----------
        name : str
            name of the particle container

        Raises
        ------
        KeyError if the particle container doesn't exists.

        """
        if name in self._components.keys():
            self._remove_component(name)
        elif name in self._cells.keys():
            self._remove_cell(name)
        else:
            raise KeyError('Particle Container "{0}" not found!'.format(name))

    def iter_particle_containers(self, names=None):
        """Iters over the given particle containers; if names is None, it will
        iterate over all.

        Parameters
        ----------
        names : iterable
            names of the particle containers

        Raises
        ------
        Exception if any of the containers is not inside nCad

        """
        if names:
            try:
                return self._iter_some_particle_containers(names)
            except KeyError as exception:
                raise exception
        else:
            return self._iter_all_particle_containers()

    # Additional methods ======================================================
    # =========================================================================
    def show(self, name):
        """Shows a currently added particle container.

        Parameters
        ----------
        name : str
            name of the particle container.

        Raises
        ------
        Exception if the pc doesn't exist.

        """
        if name in self._components:
            self._show_component(name)
        elif name in self._cells:
            self._show_cell(name)
        else:
            raise Exception('{} not found in nCad!'.format(name))

    def _show_component(self, name):
        """Shows a currently added component.

        Parameters
        ----------
        name : str
            name of the particle container added as component.

        Raises
        ------
        Exception if the pc doesn't exist.

        """
        self.thisptr.ShowComponent(name)

    def _show_cell(self, name):
        """Shows a currently added cell.

        Parameters
        ----------
        name : str
            name of the particle container added as cell.

        Raises
        ------
        Exception if the pc doesn't exist.

        """
        self.thisptr.ShowCell(name)

        
    # Private methods =========================================================
    # =========================================================================
    def _add_component(self, new_component, cell_name):
        if (new_component.name) in self._cells.keys():
            raise Exception('Duplicated particle container component: {}'
                            .format(new_component.name))
        cdef _NCadParticles ncpc = _NCadParticles('component')
        self._components[new_component.name] = ncpc
        ncpc.thisptr.name = new_component.name

        # NOTE: these are all dependent on the type of shape, so we pass them
        # all; using this schema we let the adapter deal with the shape and
        # we avoid wrapping all the shape classes of the API
        cdef c_ncad.CParticleContainerInfo pc_info
        if CUBA.CRYSTAL_ORIENTATION_1 in new_component.data:
            c_rot = new_component.data[CUBA.CRYSTAL_ORIENTATION_1]
            pc_info.crystal_rotation11[0] = c_rot[0][0]
            pc_info.crystal_rotation11[1] = c_rot[0][1]
            pc_info.crystal_rotation11[2] = c_rot[0][2]
            pc_info.crystal_rotation12[0] = c_rot[1][0]
            pc_info.crystal_rotation12[1] = c_rot[1][1]
            pc_info.crystal_rotation12[2] = c_rot[1][2]
            if CUBA.CRYSTAL_ORIENTATION_2 in new_component.data:
                c_rot = new_component.data[CUBA.CRYSTAL_ORIENTATION_2]
                pc_info.crystal_rotation21[0] = c_rot[0][0]
                pc_info.crystal_rotation21[1] = c_rot[0][1]
                pc_info.crystal_rotation21[2] = c_rot[0][2]
                pc_info.crystal_rotation22[0] = c_rot[1][0]
                pc_info.crystal_rotation22[1] = c_rot[1][1]
                pc_info.crystal_rotation22[2] = c_rot[1][2]
        # SHAPE_TYPE IS MANDATORY!
        pc_info.shape_info.shape = new_component.data[CUBA.SHAPE_TYPE]
        if CUBA.SHAPE_ORIENTATION_1 in new_component.data:
            s_rot = new_component.data[CUBA.SHAPE_ORIENTATION_1]
            pc_info.shape_info.rotation_axis1 = s_rot[0]
            pc_info.shape_info.shape_rotation1[0] = s_rot[1][0]
            pc_info.shape_info.shape_rotation1[1] = s_rot[1][1]
            pc_info.shape_info.shape_rotation1[2] = s_rot[1][2]
            if CUBA.SHAPE_ORIENTATION_2 in new_component.data:
                s_rot = new_component.data[CUBA.SHAPE_ORIENTATION_2]
                pc_info.shape_info.rotation_axis2 = s_rot[0]
                pc_info.shape_info.shape_rotation2[0] = s_rot[1][0]
                pc_info.shape_info.shape_rotation2[1] = s_rot[1][1]
                pc_info.shape_info.shape_rotation2[2] = s_rot[1][2]
        if CUBA.SHAPE_CENTER in new_component.data:
            center = new_component.data[CUBA.SHAPE_CENTER]
            pc_info.shape_info.centerX = center[0]
            pc_info.shape_info.centerY = center[1]
            pc_info.shape_info.centerZ = center[2]
        if CUBA.SHAPE_LENGTH in new_component.data:
            lengthXYZ = new_component.data[CUBA.SHAPE_LENGTH]
            pc_info.shape_info.lengthX = lengthXYZ[0]
            if len(lengthXYZ) == 3:
                pc_info.shape_info.lengthY = lengthXYZ[1]
                pc_info.shape_info.lengthZ = lengthXYZ[2]
        if CUBA.SHAPE_LENGTH_UC in new_component.data:
            lengthXYZUC = new_component.data[CUBA.SHAPE_LENGTH_UC]
            pc_info.shape_info.lengthXUC = lengthXYZUC[0]
            pc_info.shape_info.lengthYUC = lengthXYZUC[1]
            pc_info.shape_info.lengthZUC = lengthXYZUC[2]
        if CUBA.SHAPE_RADIUS in new_component.data:
            pc_info.shape_info.radius = new_component.data[CUBA.SHAPE_RADIUS]
        if CUBA.SHAPE_SIDE in new_component.data:
            pc_info.shape_info.side = new_component.data[CUBA.SHAPE_SIDE]
        self.thisptr.AddComponent(
                <c_ncad.CNCadParticleContainer *>(ncpc.thisptr),
                cell_name,
                & pc_info
                )
        self._matchParticleContainer(new_component, ncpc)
        return self._components[ncpc.thisptr.name]

    def _get_component(self, name):
        # We need to get the information that will be updated in the
        # DataContainer back from nCad ??
        return self._components[name]

    def _remove_component(self, name):
        self.thisptr.RemoveComponent(name)
        del self._components[name]

    def _add_cell(self, new_cell):
        if (new_cell.name) in self._cells.keys():
            raise Exception('Duplicated particle container cell: {}'
                            .format(new_cell.name))
        cdef _NCadParticles ncpc = _NCadParticles('cell')
        abc = new_cell.data[CUBA.LATTICE_UC_ABC]
        a = abc[0]
        b = abc[1]
        c = abc[2]
        angles = new_cell.data[CUBA.LATTICE_UC_ANGLES]
        alpha = angles[0]
        beta = angles[1]
        gamma = angles[2]
        symmetry_gn = new_cell.data[CUBA.SYMMETRY_GROUP]
        ncpc.thisptr.name = new_cell.name
        # --------------------------------------------------------------------
        self.thisptr.AddCell(<c_ncad.CNCadParticleContainer*>(ncpc.thisptr),
                             a, b, c, alpha, beta, gamma, symmetry_gn)
        self._matchParticleContainer(new_cell, ncpc)
        self._cells[new_cell.name] = ncpc
        return ncpc

    def _get_cell(self, name):
        # We need to get the information that will be updated in the
        # DataContainer back from nCad ??
        return self._cells[name]

    def _remove_cell(self, name):
        self.thisptr.RemoveCell(name)
        del self._cells[name]

    def _iter_some_particle_containers(self, cur_names):
        for cur_name in cur_names:
            try:
                yield self.get_particle_container(cur_name)
            except KeyError as e:
                raise e

    def _iter_all_particle_containers(self):
        for cur_element in self._components.itervalues():
            yield cur_element
        for cur_element in self._cells.itervalues():
            yield cur_element

    def _synch_from_ptr(self):
        pass

    cdef _matchParticleContainer(self, pc_from, _NCadParticles pc_to):
        try:
            self._copy_thisptr(pc_from.thisptr, pc_to)
        except AttributeError as exc:
            pass
        pc_to._data = pc_from.data
        for p in pc_from.iter_particles():
            pc_to.add_particle(p)
        for b in pc_from.iter_bonds():
            pc_to.add_bond(b)

    cdef _copy_thisptr(self, ptr_from, pc_to):
        cdef res = ptr_from.GetCopy()
        pc_to.thisptr = res

    cdef _newAtomsFromAssembly(self, c_ncad.CNCadParticleContainer *pc_from,
                               pc_to):
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadParticle*].iterator it
        it = pc_from.particles.begin()
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadParticle*].iterator end
        end = pc_from.particles.end()
        cdef c_ncad.CParticleInfo *particle_info = NULL
        cdef c_ncad.CNCadParticle *cur_particle
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadParticle*] new_particles
        cdef map[long long unsigned int, c_ncad.ID_TYPE] new_particles_reverse_ids
        while it != end:
            cur_particle = deref(it).second
            particle_info = self.thisptr.GetAssemblyParticleInfo(
                cur_particle.ID)
            new_particle = p.Particle(coordinates=(particle_info.x,
                                                   particle_info.y,
                                                   particle_info.z))
            new_particle.data[CUBA.CHEMICAL_SPECIE] = particle_info.specie
            new_particle.data[CUBA.LABEL] = particle_info.label
            pc_to.add_particle(new_particle)
            # Add to the component!
            new_id = new_particle.uid
            # cur_particle.Simphony_ID = new_id.hex
            simphony_id = new_id.hex
            new_particles[simphony_id] = cur_particle
            new_particles_reverse_ids[cur_particle.ID] = simphony_id
            # print "HERETHERE ", simphony_id, cur_particle.ID
            self.thisptr.ProcessAssemblyParticle(cur_particle, simphony_id)
            inc(it)
            c_ncad.delete_pointer(particle_info)
        pc_from.particles = new_particles
        pc_from.particles_reverse_ids = new_particles_reverse_ids
        return pc_to

    cdef _newBondsFromAssembly(self, c_ncad.CNCadParticleContainer *pc_from,
                               pc_to):
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadParticle*].iterator it1
        it1 = pc_from.particles.begin()
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadParticle*].iterator it2
        end1 = pc_from.particles.end()
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadBond*].iterator it
        it = pc_from.bonds.begin()
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadBond*].iterator end
        end = pc_from.bonds.end()
        cdef c_ncad.CNCadBond *cur_bond
        cdef map[c_ncad.ID_TYPE, c_ncad.CNCadBond*] new_bonds
        while it != end:
            cur_bond = deref(it).second
            # print cur_bond.atom1, cur_bond.atom2
            new_bond = p.Bond(particles=(uuid.UUID(hex=cur_bond.atom1),
                                         uuid.UUID(hex=cur_bond.atom2)))
            pc_to.add_bond(new_bond)
            # Add to the component!
            new_id = new_bond.uid
            # cur_bond.Simphony_ID = new_id.hex
            simphony_id = new_id.hex
            new_bonds[simphony_id] = cur_bond
            self.thisptr.ProcessAssemblyBond(cur_bond, simphony_id)
            inc(it)
        pc_from.bonds = new_bonds
        return pc_to
    # =========================================================================
    # =========================================================================
