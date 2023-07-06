AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  CaloGeomMapping.h

lib_LTLIBRARIES = \
  libCaloGeomMapping.la

libCaloGeomMapping_la_SOURCES = \
  CaloGeomMapping.cc

libCaloGeomMapping_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lcalo_io \
  -lcdbobjects
  # -lRawTowerGeomContainer \
  # -lRawTowerGeomv1 \
  # -lRawTowerGeomContainer_Cylinderv1

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libCaloGeomMapping.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
