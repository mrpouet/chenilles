DISTCLEANFILES= Makefile.in
bin_PROGRAMS = ArtEditor

ArtEditor_DEPENDENCIES= $(top_builddir)/src/lib/libchenilles.la

ArtEditor_SOURCES= gtksdl.h			\
	gtksdl.cpp				\
	info_dialog.h				\
	info_dialog.cpp				\
	editor_ui.h				\
	editor_ui.cpp				\
	project.h				\
	project_map.h				\
	project_map.cpp				\
	editor.h				\
	editor.cpp				\
	main.cpp

ArtEditor_CXXFLAGS= @CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@
ArtEditor_CPPFLAGS= -I$(top_builddir)/src/lib	\
	-I$(top_builddir)/src
ArtEditor_LDADD= ${ArtEditor_DEPENDENCIES}

