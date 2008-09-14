bin_PROGRAMS = chenilles ArtEditor

chenilles_DEPENDENCIES= tools/point.h		\
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
	config/xml_writer.h			\
	config/xml_writer.cpp			\
	interface/hmi.h				\
	interface/hmi.cpp			\
	interface/camera.h			\
	interface/camera.cpp			\
	map/map_exception.h			\
	map/map.h				\
	map/map.cpp				

chenilles_SOURCES= ${chenilles_DEPENDENCIES}	\
	main.cpp

ArtEditor_SOURCES= ${chenilles_DEPENDENCIES} 	\
	ArtEditor.cpp

AM_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@
AM_CPPFLAGS=@CPPFLAGS@
AM_CXXFLAGS=@CXXFLAGS@ @LIBXMLPP_CFLAGS@ -I..
