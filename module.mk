MODULE := engines/picture

MODULE_OBJS = \
	animation.o \
	detection.o \
	menu.o \
	microtiles.o \
	movie.o \
	music.o \
	palette.o \
	picture.o \
	render.o \
	resource.o \
	saveload.o \
	screen.o \
	script.o \
	segmap.o \
	sound.o \
	sprite.o


# This module can be built as a plugin
ifeq ($(ENABLE_PICTURE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
