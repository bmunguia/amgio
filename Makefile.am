pkg_cppflags  = @INRIA_CPPFLAGS@ -I$(srcdir)
pkg_sources = \
        legacy_sources/v6/libmesh6.h \
        legacy_sources/v6/libmesh6.c

######################################################################
AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS  = $(pkg_cppflags) # any defined package-specific include paths
AM_CFLAGS    =
AM_CXXFLAGS  =
AM_LDFLAGS   =

noinst_LIBRARIES = libMeshb.a

libMeshb_a_SOURCES  = $(pkg_sources)
libMeshb_a_CPPFLAGS = $(CPPFLAGS_DBG) $(AM_CPPFLAGS)
libMeshb_a_CXXFLAGS = $(CXXFLAGS_DBG)
libMeshb_a_CFLAGS   = $(CFLAGS_DBG)