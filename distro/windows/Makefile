ARCH = i686
TARGET=$(ARCH)-w64-mingw32
MINGW_PREFIX=/usr/$(TARGET)

GLPORTAL_SRC = $(PWD)/../..
GLPORTAL_BUILD = $(GLPORTAL_SRC)/build-$(TARGET)
GLPORTAL_PREFIX = $(GLPORTAL_BUILD)/install
GLPORTAL_ZIP = $(GLPORTAL_BUILD)/glportal

all: glportal-zip

glportal-config:
	-mkdir -p $(GLPORTAL_BUILD)
	cd $(GLPORTAL_BUILD) && rm -f CMakeCache.txt && $(TARGET)-cmake \
	-DCMAKE_INSTALL_PREFIX=$(GLPORTAL_PREFIX) \
	$(GLPORTAL_SRC)
	touch glportal-config

glportal-build: glportal-config
	cd $(GLPORTAL_BUILD) && make -j2
	
glportal-install: glportal-build
	cd $(GLPORTAL_BUILD) && make install
	$(TARGET)-strip $(GLPORTAL_PREFIX)/bin/glportal.exe
	
glportal-zip: glportal-install
	-mkdir -p $(GLPORTAL_ZIP)
	cp $(GLPORTAL_PREFIX)/bin/glportal.exe $(GLPORTAL_ZIP)
	cp -r $(GLPORTAL_PREFIX)/share/glportal/data $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/libwinpthread-1.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/libgcc_s_*.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/libstdc++-6.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/zlib1.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/SDL2.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/SDL2_mixer.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/libassimp.dll $(GLPORTAL_ZIP)
	cp $(MINGW_PREFIX)/bin/glew32.dll $(GLPORTAL_ZIP)
	bsdtar cf glportal.zip --format zip -C $(GLPORTAL_BUILD) glportal
	
clean:
	rm -rf $(GLPORTAL_BUILD)
	rm -f glportal-*

	