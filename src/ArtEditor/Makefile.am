bin_PROGRAMS = ArtEditor

ArtEditor_SOURCES= tools/point.h		\
	../tools/point.cpp			\
	tools/rectangle.h			\
	../tools/rectangle.cpp			\
	tools/base.h				\
	tools/singleton.h			\
	tools/game_exception.h			\
	game/timer.h				\
	../game/timer.cpp			\
	graphics/color.h			\
	../graphics/color.cpp			\
	graphics/sdl_exception.h		\
	graphics/surface.h			\
	../graphics/surface.cpp			\
	graphics/animation.h			\
	../graphics/animation.cpp		\
	config/xml_exception.h			\
	config/xml_parser.h			\
	../config/xml_parser.cpp		\
	config/xml_writer.h			\
	../config/xml_writer.cpp		\
	interface/hmi.h				\
	../interface/hmi.cpp			\
	interface/camera.h			\
	../interface/camera.cpp			\
	map/map_exception.h			\
	map/map.h				\
	../map/map.cpp				\
	gtksdl.h				\
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

AM_LDFLAGS= @LDFLAGS@ @LIBXMLPP_LIBS@ @GTKMM_LIBS@
AM_CPPFLAGS=@CPPFLAGS@
AM_CXXFLAGS=@CXXFLAGS@ @LIBXMLPP_CFLAGS@ @GTKMM_CFLAGS@ -I.. -I../..
