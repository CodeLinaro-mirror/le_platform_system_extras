ACLOCAL_AMFLAGS = -I m4

$(info CONF_ENABLED is $(CONF_ENABLED))

root_etcconfdir = "/etc/conf"
root_etcconf_SCRIPTS = ../configs/media-encryption/$(CONF_ENABLED)

BUILD_COMPONENTS  = libs
BUILD_COMPONENTS += tests

SUBDIRS = $(BUILD_COMPONENTS)

install-data-hook:
	cd $(DESTDIR)/$(root_etcconfdir) && \
	mv $(CONF_ENABLED) media-encryption.conf
