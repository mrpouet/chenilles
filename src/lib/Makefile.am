SUBDIRS= private
DISTCLEANFILES= Makefile.in
lib_LTLIBRARIES= libchenilles.la

libchenilles_la_SOURCES=		\
	point.cpp			\
	rectangle.cpp			\
	timer.cpp			\
	color.cpp			\
	surface.cpp			\
	animation.cpp			\
	xml_parser.cpp			\
	xml_writer.cpp			\
	hmi.cpp				\
	camera.cpp			\
	map.cpp

installed_headersdir= $(includedir)/chenilles
installed_headers_DATA= 		\
	point.h				\
	rectangle.h			\
	singleton.h			\
	game_exception.h		\
	timer.h				\
	color.h				\
	surface.h			\
	animation.h			\
	xml_parser.h			\
	xml_writer.h			\
	hmi.h				\
	camera.h			\
	map.h

libchenilles_la_CPPFLAGS= -I$(top_builddir)/src
libchenilles_la_CXXFLAGS= @CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@
libchenilles_la_LDFLAGS= -version-info @version_info@
libchenilles_la_LIBADD= @LDFLAGS@ @LIBXMLPP_LIBS@ @GTKMM_LIBS@


