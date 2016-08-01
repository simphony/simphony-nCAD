from simphony.engine.extension import ABCEngineExtension  
from simphony.engine.extension import EngineInterface  

from simncad.ncad import nCad
from simncad.auxiliar.celldata_parser import read_cd
from simncad.auxiliar.ncad_types import SHAPE_TYPE, AXIS_TYPE, SYMMETRY_GROUP

__all__ = {'nCad', 'read_cd', 'SHAPE_TYPE', 'AXIS_TYPE', 'SYMMETRY_GROUP', 'SimNCadExtension'}


class SimNCadExtension(ABCEngineExtension):
    """Simphony-NCad extension.

    This extension provides support for the nCad wrapper engine.
    """

    def get_supported_engines(self):
        """Get metadata about supported engines.

        Returns
        -------
        list: a list of EngineMetadata objects
        """
        # TODO: Add proper features as soon as the metadata classes are ready.
        ncad_features = None
        ncad_eng = self.create_engine_metadata('NCAD',
                                           ncad_features,
                                           [EngineInterface.Internal])
        return [ncad_eng]

    def create_wrapper(self, cuds, engine_name, engine_interface):
        """Creates a wrapper to the requested engine.

        Parameters
        ----------
        cuds: CUDS
          CUDS computational model data
        engine_name: str
          name of the engine, must be supported by this extension
        engine_interface: EngineInterface
          the interface to interact with engine

        Returns
        -------
        ABCEngineExtension: A wrapper configured with cuds and ready to run
        """
        # We only support internal
        if engine_name == 'NCAD':
            return nCad(cuds=cuds)
        else:
            raise Exception('Only nCad engine is supported. '
                            'Unsupported engine: %s', engine_name)
