#include "layerstore.h"

bool LayerStore::drag_data_received_vfunc (const TreeModel::Path & dest,
					   const SelectionData & selection_data)
{
    // First we need to called super class method
    // to extract some informations.
    bool ret = ListStore::drag_data_received_vfunc (dest, selection_data);
    TreeModel::Row row_dest = *(--get_iter (dest));
    TreeModel::Row src = *get_iter (dest);

    do_drop_end (row_dest, src);

    return ret;
}
