ACLOCAL_AMFLAGS = -I m4

$(info CONF_ENABLED is $(CONF_ENABLED))

# Destination directory
root_etcconfdir = /etc/conf

# IMPORTANT:
# Handle all config installs manually in install-data-hook
# so that CONF_ENABLED can be either:
#   - legacy filename (kera.conf or sun.conf etc.)
#   - prefix (kera or sun or neo etc.)

BUILD_COMPONENTS  = libs
BUILD_COMPONENTS += tests

SUBDIRS = $(BUILD_COMPONENTS)

# install-data-hook logic:$
# - Legacy case (CONF_ENABLED ends with .conf):$
#     install <conf> and rename to media-encryption.conf$
# - Prefix-based case:$
#     install <prefix>_emmc.conf -> media-encryption_emmc.conf$
#     install <prefix>_ufs.conf  -> media-encryption_ufs.conf$
#     optional fallback <prefix>.conf -> media-encryption.conf$

install-data-hook:
	mkdir -p $(DESTDIR)/$(root_etcconfdir) ; \
	cd $(DESTDIR)/$(root_etcconfdir) || exit 1 ; \
		if echo "$(CONF_ENABLED)" | grep -q '\.conf$$'; then \
			echo "Legacy config detected: $(CONF_ENABLED)" ; \
	     $(INSTALL_DATA) \
	       "$(abs_top_srcdir)/../configs/media-encryption/$(CONF_ENABLED)" \
	       "$(DESTDIR)/$(root_etcconfdir)/$(CONF_ENABLED)" ; \
	      mv "$(CONF_ENABLED)" media-encryption.conf ; \
	  else \
	       echo "Prefix-based config detected: $(CONF_ENABLED)" ; \
	       for s in emmc ufs nand ; do \
	           f="$(CONF_ENABLED)_$${s}.conf" ; \
	           if test -f "$(abs_top_srcdir)/../configs/media-encryption/$${f}"; then \
	               echo "Installing $${f} as media-encryption_$${s}.conf" ; \
	               $(INSTALL_DATA) \
	                 "$(abs_top_srcdir)/../configs/media-encryption/$${f}" \
	                 "$(DESTDIR)/$(root_etcconfdir)/media-encryption_$${s}.conf" ; \
	           fi ; \
	       done ; \
				 \
	       if test -f "$(abs_top_srcdir)/../configs/media-encryption/$(CONF_ENABLED).conf"; then \
	           echo "Installing fallback $(CONF_ENABLED).conf as media-encryption.conf" ; \
	           $(INSTALL_DATA) \
	             "$(abs_top_srcdir)/../configs/media-encryption/$(CONF_ENABLED).conf" \
	             "$(DESTDIR)/$(root_etcconfdir)/media-encryption.conf" ; \
	       fi ; \
	  fi
