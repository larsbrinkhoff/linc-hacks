all: picture

picture: picture.o lodepng.o

picture.o:: lodepng.h

lodepng.c: lodepng/lodepng.cpp
	cp $< $@

lodepng.h: lodepng/lodepng.h
	cp $< $@

lodepng/lodepng.h:
	git submodule sync --recursive
	git submodule update --init --recursive
