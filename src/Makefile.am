SUBDIRS= lib ArtEditor

bin_PROGRAMS=chenilles

chenilles_DEPENDENCIES= lib/libchenilles.la

chenilles_SOURCES= main.cpp

AM_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@ @GTKMM_LIBS@
AM_CPPFLAGS=@CPPFLAGS@ \
	-I$(top_builddir)

AM_CXXFLAGS=@CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@

if NO_STD_LIBDIR
install-exec-hook:
	ldconfig
endif