Name: fabtests
Version: 1.3.0
Release: 1%{?dist}
Summary: Test suite for libfabric API
Group: System Environment/Libraries
License: GPLv2 or BSD
Url: http://www.github.com/ofiwg/fabtests
Source: https://github.com/ofiwg/%{name}/releases/download/v%{version}/%{name}-%{version}.tar.bz2
BuildRequires: libfabric
BuildRequires: libfabric-devel

%description
Fabtests provides a set of examples that uses libfabric - a high-performance
fabric software library.

%prep
%setup -q

%build
%configure %{?_with_libfabric}
make %{?_smp_mflags}

%install
%make_install
# remove unpackaged files from the buildroot
rm -f %{buildroot}%{_libdir}/*.la

%files
%{_bindir}/*
%{_mandir}/man7/*
%doc AUTHORS COPYING README
%{_prefix}/share/fabtests/test_configs

%changelog
* Tue May 31 2016 Honggang Li <honli@redhat.com> - 1.3.0-1
- Rebase to upstream release 1.3.0.
- Related: bz1280146

* Fri Aug 14 2015 Honggang Li <honli@redhat.com> - 1.1.0-1
- Rebase to upstream 1.1.0
- Resolves: bz1253513

* Sat Aug 08 2015 Michal Schmidt <mschmidt@redhat.com> - 1.1.0-0.2.rc1
- Packaging Guidelines conformance fixes and spec file cleanups
- Fix libfabric dependency
- Related: bz1235270

* Wed Aug  5 2015 Honggang Li <honli@redhat.com> - 1.1.0-0.1.rc1
- Initial build for RHEL-7.2
- Related: bz1235270

* Sun May 3 2015 Open Fabrics Interfaces Working Group <ofiwg@lists.openfabrics.org> 1.0.0
- Release 1.0.0
