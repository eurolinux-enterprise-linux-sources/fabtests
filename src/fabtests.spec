Name: fabtests
Version: 1.5.3
Release: 1%{?dist}
Summary: Test suite for libfabric API
Group: System Environment/Libraries
License: GPLv2 or BSD
Url: http://www.github.com/ofiwg/fabtests
Source: http://www.github.org/ofiwg/%{name}/releases/download/v{%version}/%{name}-%{version}.tar.bz2
Requires: libfabric
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
Fabtests provides a set of examples that uses libfabric - a high-performance fabric software library.

%prep
%setup -q -n %{name}-%{version}

%build
%configure %{?_with_libfabric}
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%makeinstall installdirs
# remove unpackaged files from the buildroot
rm -f %{buildroot}%{_libdir}/*.la

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_mandir}/man7/*
%{_datadir}/%{name}/test_configs/*
%doc AUTHORS COPYING README

%changelog
* Sun May 3 2015 Open Fabrics Interfaces Working Group <ofiwg@lists.openfabrics.org> 1.0.0
- Release 1.0.0
