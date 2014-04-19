CC=g++
CXXFLAGS=-I. -ISTEP_SDK/ -Wall -O2 -m32
OBJECTS=STEP_mid.o SMFUtil.o STEP_SDK/STEPlugin.o
RESOURCE=resource.o
ARCHIVE=libSTEP_mid.a
TARGET=STEP_mid.ste
EXPFILE=STEP_mid.exp

all: $(TARGET)

resource.rc: resource.m4
	m4 -D VERSION=$(shell git describe --tags) $< > $@

$(RESOURCE): resource.rc resource.h
	windres -o $@ $<

$(EXPFILE): $(OBJECTS)
	dlltool -e $@ -U $<

$(TARGET): $(OBJECTS) $(EXPFILE) $(RESOURCE)
	ar rcs $(ARCHIVE) $(OBJECTS)
	$(CC) -shared -s -o $@ $(EXPFILE) $(RESOURCE) -nostartfiles -static $(ARCHIVE)

clean:
	-rm $(OBJECTS) $(TARGET) $(EXPFILE) $(RESOURCE) $(ARCHIVE)

distclean: clean
	-rm resource.rc

.PHONY: all clean distclean
