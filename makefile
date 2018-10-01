CXX = /usr/bin/g++-7

CPPVER = c++17

APP = app

OUTDIR = out
BINDIR = bin

LIBDIR = lib
LIBS = 

COMPOPT = -ldl -lSDL2 -pthread
LINKOPT = 

DBGCOMPOPT = 
DBGLINKOPT = 

RELCOMPOPT = 
RELLINKOPT = 

SOURCES = src lib lib_imgui lib_imgui_misc_stl include_lak

src_SRC = ./src
src_OBJ = source.cpp nodegraph.cpp
src_INC = include lib lib/imgui lib/imgui/misc/stl /usr/include/SDL2

lib_SRC = ./lib
lib_OBJ = gl3w.c imgui_impl_sdl_gl3.cpp 
lib_INC = include lib/imgui /usr/include/SDL2

lib_imgui_SRC = ./lib/imgui
lib_imgui_OBJ = imgui_draw.cpp imgui_demo.cpp imgui.cpp
lib_imgui_INC = include

lib_imgui_misc_stl_SRC = ./lib/imgui/misc/stl
lib_imgui_misc_stl_OBJ = imgui_stl.cpp 
lib_imgui_misc_stl_INC = include lib/imgui

include_lak_SRC = ./include/lak
include_lak_OBJ = queue.cpp gizmo.cpp graphics.cpp json.cpp obj.cpp stream_util.cpp stride_vector.cpp
include_lak_INC = include /usr/include/SDL2

# -------------------
# Start build script:
# -------------------
ALL_OBJ = $(foreach src,$(SOURCES),$(foreach obj,$($(src)_OBJ),$(BINDIR)/$(obj).o))

.PHONY: debug
debug: $(foreach obj,$(ALL_OBJ),debug-$(obj))
	$(call LINK_TEMPLATE,$(DBGLINKOPT))

release: $(foreach obj,$(ALL_OBJ),release-$(obj))
	$(call LINK_TEMPLATE,$(RELLINKOPT))

define LINK_TEMPLATE =
$(CXX) -std=$(CPPVER) -o $(OUTDIR)/$(APP) $(ALL_OBJ) $(foreach libdir,$(LIBDIR),-L$(libdir)) $(foreach lib,$(LIBS),-l$(lib)) $(COMPOPT) $(1)
endef

define COMPILE_TEMPLATE =
debug-$(2)/$(3).o: $(1)/$(3)
	$(CXX) -std=$(CPPVER) -c -o $(2)/$(3).o $(1)/$(3) $(4) $(DBGCOMPOPT)
release-$(2)/$(3).o: $(1)/$(3)
	$(CXX) -std=$(CPPVER) -c -o $(2)/$(3).o $(1)/$(3) $(4) $(RELCOMPOPT)
endef

$(foreach src,$(SOURCES),$(foreach obj,$($(src)_OBJ),$(eval $(call COMPILE_TEMPLATE,$($(src)_SRC),$(BINDIR),$(obj),$(foreach inc,$($(src)_INC),-I$(inc))))))

clean:
	rm -f $(ALL_OBJ)