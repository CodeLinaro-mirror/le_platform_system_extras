ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = generate_verity_key \
               verify_boot_signature \
               build_verity_tree

generate_verity_key_SOURCES   = $(srcdir)/generate_verity_key.c
generate_verity_key_CFLAGS    = -I$(srcdir)/../../core/include
generate_verity_key_LDFLAGS   = -lcrypto

verify_boot_signature_SOURCES = $(srcdir)/verify_boot_signature.c
verify_boot_signature_CFLAGS  = -I$(srcdir)/../ext4_utils -I$(srcdir)/../../core/mkbootimg
verify_boot_signature_LDFLAGS = -lcrypto

build_verity_tree_SOURCES     = $(srcdir)/build_verity_tree.cpp
build_verity_tree_CPPFLAGS    = -I@SPARSE_CFLAGS@ -I@Z_CFLAGS@ -Wall
build_verity_tree_LDADD       = @Z_LIBS@
build_verity_tree_LDFLAGS     = -lcrypto @SPARSE_LIBS@

veritydir       = ${bindir}
verity_SCRIPTS  = verity_signer
verity_SCRIPTS += build_verity_metadata.py
verity_SCRIPTS += verity.pk8
verity_SCRIPTS += verity.x509.pem
