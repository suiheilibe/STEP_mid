FILES=readme.txt COPYING src/STEP_mid.ste
PRODUCT=STEP_mid
DISTFILE=$(PRODUCT)-$(shell git describe --tags | sed -e 's/^v//g').zip

dist: all
	-mkdir dist
	cd dist ; rm -r * ; mkdir $(PRODUCT) ; cd ../ ; \
	cp $(FILES) dist/$(PRODUCT) ; cd dist ; \
	zip -9 $(DISTFILE) $(PRODUCT)/*

all clean distclean:
	make -C src $@
