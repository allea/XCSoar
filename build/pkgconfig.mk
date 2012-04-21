PKG_CONFIG = pkg-config

ifeq ($(TARGET_IS_DARWIN),y)
  PKG_CONFIG += --static
endif
PKG_CONFIG := $(PKG_PREFIX) $(PKG_CONFIG)

# Generates a pkg-config lookup for a library.
#
# Example: $(eval $(call CURL,libcurl >= 2.21))
#
# Arguments: PREFIX, SPEC
#
# PREFIX is a prefix for variables that will hold the results.  This
# function will append "_CPPFLAGS" (pkg-config --cflags) and "_LDLIBS"
# (pkg-config --libs).
#
# SPEC is the pkg-config package specification.
#
define pkg-config-library

$(1)_CPPFLAGS := $$(shell $$(PKG_CONFIG) --cflags $(2))
$(1)_LDLIBS := $$(shell $$(PKG_CONFIG) --libs $(2))

ifeq ($$($(1)_CPPFLAGS)$$($(1)_LDLIBS),)
$$(error library not found: $(2))
endif

endef
