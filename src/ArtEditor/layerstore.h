#ifndef __LAYER_VIEW_H__
#define __LAYER_VIEW_H__

#include <gtkmm/liststore.h>

using namespace Gtk;

class LayerStore:public ListStore
{

  public:

    static inline Glib::RefPtr < LayerStore >
    create (const TreeModelColumnRecord & columns)
    {
	return Glib::RefPtr < LayerStore > (new LayerStore (columns));
    }

    // signal "drop_end" accessor/mutator
    // in RW.
    sigc::signal < void, const TreeModel::Row &,const TreeModel::Row & >&
    signal_drop_end (void)
    {
	return m_signal_drop_end;
    }

  private:

    explicit LayerStore (const TreeModelColumnRecord & columns):ListStore (columns) {};

    // emits the "drop_end" signal which is normally emitted after a drag&drop
    // @param dest The Row after which drag row was inserted (by gtk itself on user ask).
    // @param row The dropped row.
    inline void do_drop_end (const TreeModel::Row & dest,
			     const TreeModel::Row & row) const
    {
	m_signal_drop_end.emit (dest, row);
    }

    // see gtkmmm/treedragdest header file for more details.
    bool drag_data_received_vfunc (const TreeModel::Path & dest,
				   const SelectionData & selection_data);

    sigc::signal < void, const TreeModel::Row &, 
      const TreeModel::Row & > m_signal_drop_end;


};

#endif
