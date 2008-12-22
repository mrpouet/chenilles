SUBDIRS= lib ArtEditor

bin_PROGRAMS=chenilles

chenilles_DEPENDENCIES= lib/libchenilles.la

chenilles_SOURCES= tools/fall_vector.cpp	\
	object/drawable_object.cpp		\
	game/game.cpp				\
	units/unit.cpp				\
	main.cpp

chenilles_LDADD= ${chenilles_DEPENDENCIES}

AM_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@ @GTKMM_LIBS@
AM_CPPFLAGS=@CPPFLAGS@ 			\
	-I$(top_builddir)		\
	-I$(top_builddir)/src/lib	\
	-I.

AM_CXXFLAGS=@CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@

if NO_STD_LIBDIR
install-exec-hook:
	ldconfig
endif