DISTCLEANFILES= Makefile.in
lib_LTLIBRARIES= libchenilles.la

libchenilles_la_SOURCES= tools/point.cpp	\
	tools/rectangle.cpp			\
	game/timer.cpp				\
	graphics/color.cpp			\
	graphics/surface.cpp			\
	graphics/animation.cpp			\
	config/xml_parser.cpp			\
	config/xml_writer.cpp			\
	interface/hmi.cpp			\
	interface/camera.cpp			\
	map/map.cpp

include_HEADERS= tools/point.h			\
	tools/rectangle.h			\
	tools/singleton.h			\
	tools/game_exception.h			\
	game/timer.h				\
	game/timer.cpp				\
	graphics/color.h			\
	graphics/sdl_exception.h		\
	graphics/surface.h			\
	graphics/animation.h			\
	config/xml_exception.h			\
	config/xml_parser.h			\
	config/xml_writer.h			\
	interface/hmi.h				\
	interface/camera.h			\
	map/map_exception.h			\
	map/map.h

libchenilles_la_CPPFLAGS= -I$(top_builddir)/src
libchenilles_la_CXXFLAGS= @CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@
libchenilles_la_LDFLAGS= -version-info @version_info@
libchenilles_la_LIBADD= @LDFLAGS@ @LIBXMLPP_LIBS@ @GTKMM_LIBS@


