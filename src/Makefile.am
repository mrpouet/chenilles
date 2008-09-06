bin_PROGRAMS = chenilles

chenilles_SOURCES= tools/point.h				\
	tools/point.cpp				\
	tools/rectangle.h			\
	tools/rectangle.cpp			\
	tools/base.h				\
	tools/singleton.h			\
	tools/game_exception.h			\
	game/timer.h				\
	game/timer.cpp				\
	graphics/color.h			\
	graphics/color.cpp			\
	graphics/sdl_exception.h		\
	graphics/surface.h			\
	graphics/surface.cpp			\
	graphics/animation.h			\
	graphics/animation.cpp			\
	config/xml_exception.h			\
	config/xml_parser.h			\
	config/xml_parser.cpp			\
	map/map_exception.h			\
	map/map.h				\
	map/map.cpp				\
	main.cpp

chenilles_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@
chenilles_CPPFLAGS=@CPPFLAGS@
chenilles_CXXFLAGS=@CXXFLAGS@ @LIBXMLPP_CFLAGS@ -I..
