##  This file is part of GPar2 (a PAR 2.0 compatible file verification and
##  repair GUI). See http://parchive.sourceforge.net for details of PAR 2.0.
##
##  Copyright (c) 2005 Francois LESUEUR
##
##  GPar2 is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
##  GPar2 is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##                                               -*- Autoconf -*-
## Process this file with autoconf to produce a configure script.

bin_PROGRAMS = gpar2

gpar2_SOURCES = gpar2.cc gpar2.h \
	mainwindow.cc mainwindow.h

AM_LDFLAGS = -lstdc++ -lsigc-2.0 `pkg-config gtkmm-2.4 --libs` -lpar2
INCLUDES = `pkg-config sigc++-2.0 gtkmm-2.4 --cflags`
AM_CXXFLAGS = -DLOCALEDIR='"$(prefix)/share/locale"'

EXTRA_DIST = config.rpath gnome-logo-icon-transparent.png gpar2.xml



SUBDIRS = po m4

ACLOCAL_AMFLAGS = -I m4
