bin_PROGRAMS= collidemap chenilles-viewer

PREFIX=../src

collidemap_DEPS= tools/point.h		\
	$(PREFIX)/tools/point.cpp		\
	tools/rectangle.h			\
	$(PREFIX)/tools/rectangle.cpp		\
	tools/base.h				\
	tools/singleton.h			\
	game/timer.h				\
	$(PREFIX)/game/timer.cpp		\
	graphics/color.h			\
	$(PREFIX)/graphics/color.cpp		\
	graphics/surface.h			\
	$(PREFIX)/graphics/surface.cpp		\
	graphics/animation.h			\
	$(PREFIX)/graphics/animation.cpp
collidemap_SOURCES= ${collidemap_DEPS}		\
	collidemap.cpp

chenilles_viewer_SOURCES= ${collidemap_DEPS} 	\
	config/xml_parser.h			\
	$(PREFIX)/config/xml_parser.cpp		\
	map/map.h				\
	$(PREFIX)/map/map.cpp			\
	chenilles-viewer.cpp

chenilles_viewer_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@
chenilles_viewer_CXXFLAGS= @CXXFLAGS@ @LIBXMLPP_CFLAGS@ -I$(PREFIX)
chenilles_viewer_CPPFLAGS= @CPPFLAGS@


