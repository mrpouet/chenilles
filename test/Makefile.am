bin_PROGRAMS= collidemap

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
AM_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@
AM_CXXFLAGS= @CXXFLAGS@ @LIBXMLPP_CFLAGS@ -I$(PREFIX)
AM_CPPFLAGS= @CPPFLAGS@


