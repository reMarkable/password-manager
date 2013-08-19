Name: nemo-password-manager
Version: 0.0.1
Release: 1
Summary: D-Bus Service for changing and generating passwords

%define dbus_service_name org.nemo.passwordmanager

Group: System Environment/Daemon
License: GPL
URL: https://github.com/nemomobile/password-manager
Source: %{name}-%{version}.tar.gz

BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pam-devel

%description
Password Manager manages user account passwords for developer mode.
It can generate random passwords or set user-supplied passwords.

%prep
%setup -q -n %{name}-%{version}


%build
%qmake5
make

%install
rm -rf %{buildroot}
%qmake5_install

%clean
rm -rf %{buildroot}


%post
# TODO: Ask running daemon to quit

%postun
# TODO: Ask running daemon to quit


%files
%defattr(-,root,root,-)
%doc README
%{_bindir}/%{name}
%{_datadir}/dbus-1/system-services/%{dbus_service_name}.service
%{_sysconfdir}/dbus-1/system.d/%{dbus_service_name}.conf
