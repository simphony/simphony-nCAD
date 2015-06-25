#ifndef __VISUALIZER_SIMPHONY__H__
#define __VISUALIZER_SIMPHONY__H__

/**@pkg _SIMPHONY_ADAPTER*/

class VisualizerSimphony
/**Class representing a Simphony visualizer.*/
{
public:
    /**Constructor.*/
    VisualizerSimphony();

    /**Shows the cell CNCadCell specified as argument.
    @param data pointer to the CNCadCell to show.
    */
    void ShowCell(void * data);
    /**Shows the component CNCadComponent specified as argument.
    @param data pointer to the CNCadComponent to show.
    */
    void ShowComponent(void * data);
};
#endif