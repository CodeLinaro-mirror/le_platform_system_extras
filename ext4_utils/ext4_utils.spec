Name: ext4-utils
Version: 1.0
Release: r0
Summary: Android ext4-utils tools

License: Apache-2.0

URL: https://www.codelinaro.org/
#Source0: ext4-utils-1.0.tar.gz
Source0: %{name}-%{version}.tar.gz

BuildRequires: autoconf automake libcutils-dev libsparse-dev libtool gcc-g++

%description
Command line tools to make sparse images from ext4 file system
images and android images(.img) with ext4 file systems.
This package contains tools like mkuserimg, ext4fixup
and make_ext4fs tools.

# subpackage named ${name}-dev -> "ext4-utils-dev"
%package dev
Summary: Android ext4-utils tools - Development files
License: Apache-2.0
Requires: %{name} = %{version}-%{release}

%description dev
Command line tools to make sparse images from ext4 file system
images and android images(.img) with ext4 file systems.
This package contains tools like mkuserimg, ext4fixup and
make_ext4fs tools. This package contains symbolic links,header
files,and related items necessary for software development.

%prep
%autosetup -n ext4_utils

%build
autoreconf -if
%configure --with-core-includes=%{_builddir}/ext4_utils/

%make_build

%install
%make_install

%files
%{_libdir}/libext4_utils.a
%{_libdir}/libext4_utils.la
%{_libdir}/libext4_utils.so.0
%{_libdir}/libext4_utils.so.0.0.0

%files dev
%dir %{_includedir}/ext4_utils
%{_includedir}/ext4_utils/ext4.h
%{_includedir}/ext4_utils/ext4_crypt_init_extensions.h
%{_includedir}/ext4_utils/ext4_extents.h
%{_includedir}/ext4_utils/ext4_kernel_headers.h
%{_includedir}/ext4_utils/ext4_sb.h
%{_includedir}/ext4_utils/ext4_utils.h
%{_includedir}/ext4_utils/jbd2.h
%{_includedir}/ext4_utils/make_ext4fs.h
%{_includedir}/ext4_utils/wipe.h
%{_includedir}/ext4_utils/xattr.h
%{_libdir}/libext4_utils.so
%{_libdir}/pkgconfig/ext4_utils.pc
