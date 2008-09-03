bin_PROGRAMS= collidemap

PREFIX=../src
collidemap_SOURCES= tools/point.h		\
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
	$(PREFIX)/graphics/animation.cpp	\
	collidemap.cpp

AM_CXXFLAGS= @CXXFLAGS@ -I$(PREFIX)
AM_CPPFLAGS= @CPPFLAGS@
AM_LDFLAGS= @LDFLAGS@

