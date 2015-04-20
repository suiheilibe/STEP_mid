CC=g++
CXXFLAGS=-I. -ISTEP_SDK/ -Wall -Os -m32
OBJECTS=STEP_mid.o SMFUtil.o STEP_SDK/STEPlugin.o
RESOURCE=resource.o
ARCHIVE=libSTEP_mid.a
TARGET=STEP_mid.ste
EXPFILE=STEP_mid.exp
WINDRES=windres
DLLTOOL=dlltool
AR=ar

all: $(TARGET)

resource.rc: resource.m4
	m4 -D VERSION=$(shell git describe --tags) $< > $@

$(RESOURCE): resource.rc resource.h
	$(WINDRES) -o $@ $<

$(EXPFILE): $(OBJECTS)
	$(DLLTOOL) -e $@ -U $<

$(TARGET): $(OBJECTS) $(EXPFILE) $(RESOURCE)
	$(AR) rcs $(ARCHIVE) STEP_mid.o SMFUtil.o
	$(CC) -shared -s -o $@ $(EXPFILE) $(RESOURCE) STEP_SDK/STEPlugin.o $(ARCHIVE) -static

clean:
	-rm $(OBJECTS) $(TARGET) $(EXPFILE) $(RESOURCE) $(ARCHIVE) resource.rc

distclean: clean

.PHONY: all clean distclean
