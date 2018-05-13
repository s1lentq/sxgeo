NAME = sxgeo
CSSDK = sxgeo/include/cssdk
METAMOD = sxgeo/include/metamod
SRCDIR = sxgeo/src
COMPILER = g++
BIN_SUFFIX = i386
BIN_DIR = Release

OBJECTS		=	$(SRCDIR)/sxgeo/db.cpp $(SRCDIR)/sxgeo/location.cpp\
				$(SRCDIR)/amxx_natives.cpp $(SRCDIR)/amxxmodule.cpp\
				$(SRCDIR)/h_export.cpp $(SRCDIR)/meta_api.cpp $(SRCDIR)/sxgeo.cpp $(SRCDIR)/main.cpp $(SRCDIR)/precompiled.cpp

LINK		=	-static-libstdc++ -static-libgcc -Wl,--version-script=version_script.lds -Wl,--gc-sections -fno-exceptions
OPT_FLAGS	=	-O2 -funroll-loops -s -fomit-frame-pointer -fno-strict-aliasing -ffunction-sections -fdata-sections

INCLUDE		=	-I. -I$(SRCDIR) -I$(CSSDK)/common -I$(CSSDK)/dlls -I$(CSSDK)/engine -I$(CSSDK)/pm_shared -I$(CSSDK)/public -I$(CSSDK)/game_shared -I$(METAMOD)
CFLAGS		=	$(OPT_FLAGS) -Wno-write-strings
CFLAGS		+=	-g0 -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -Dlinux -D__linux__ -D_vsnprintf=vsnprintf -std=c++14 -fno-rtti -m32

OBJ_LINUX := $(OBJECTS:%.cpp=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.cpp
	$(COMPILER) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/sxgeo/src
	mkdir -p $(BIN_DIR)/sxgeo/src/sxgeo

	$(MAKE) $(NAME)

$(NAME): $(OBJ_LINUX)
	$(COMPILER) $(INCLUDE) $(CFLAGS) $(OBJ_LINUX) $(LINK) -shared -lm -o$(BIN_DIR)/$(NAME)_amxx_$(BIN_SUFFIX).so

check:
	cppcheck $(INCLUDE) --quiet --max-configs=100 -D__linux__ -DNDEBUG -DHAVE_STDINT_H .

debug:
	$(MAKE) all DEBUG=false

default: all

clean:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/sxgeo/src/*.o
	rm -rf $(BIN_DIR)/sxgeo/src/sxgeo/*.o
	rm -rf $(BIN_DIR)/$(NAME)_amxx_$(BIN_SUFFIX).so
