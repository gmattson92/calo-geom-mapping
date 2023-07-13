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
  printCaloGeom.h

lib_LTLIBRARIES = \
  libprintCaloGeom.la

libprintCaloGeom_la_SOURCES = \
  printCaloGeom.cc

libprintCaloGeom_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lHepMC \
  -lCLHEP \
  -lcalo_io \
  -lg4dst \
  -lg4eval \
  # -lg4jets_io \
  -lg4jets

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libprintCaloGeom.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
