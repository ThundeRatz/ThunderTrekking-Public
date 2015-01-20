# Makefile.in generated by automake 1.14.1 from Makefile.am.
# Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2013 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.




am__is_gnu_make = test -n '$(MAKEFILE_LIST)' && test -n '$(MAKELEVEL)'
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/trekking
pkgincludedir = $(includedir)/trekking
pkglibdir = $(libdir)/trekking
pkglibexecdir = $(libexecdir)/trekking
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
bin_PROGRAMS = i2c_features$(EXEEXT) leds_control$(EXEEXT) \
	hmc5883l$(EXEEXT) nmea$(EXEEXT) compass_dump$(EXEEXT) \
	trekking$(EXEEXT) hmc5883l-log$(EXEEXT) motors-test$(EXEEXT) \
	udp_send$(EXEEXT) udp_recv_bussola$(EXEEXT) \
	udp_recv_gps$(EXEEXT) udp_recv_proximity$(EXEEXT) \
	motors_i2c_force$(EXEEXT) \
	motors_i2c_force_alternating$(EXEEXT) calibra_bussola$(EXEEXT) \
	teste_bussola$(EXEEXT) joystick_kill$(EXEEXT) \
	udp_recv_hmc5883l$(EXEEXT) calibra_hmc5883l$(EXEEXT) \
	hmc_log_rotate$(EXEEXT)
subdir = .
DIST_COMMON = $(srcdir)/Makefile.in $(srcdir)/Makefile.am \
	$(top_srcdir)/configure $(am__configure_deps) \
	$(srcdir)/config.h.in depcomp compile install-sh missing
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
am__CONFIG_DISTCLEAN_FILES = config.status config.cache config.log \
 configure.lineno config.status.lineno
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
am__installdirs = "$(DESTDIR)$(bindir)"
PROGRAMS = $(bin_PROGRAMS)
am_calibra_bussola_OBJECTS = calibra_bussola.$(OBJEXT) i2c.$(OBJEXT) \
	joystick.$(OBJEXT) compass.$(OBJEXT) file_lock.$(OBJEXT) \
	motors.$(OBJEXT) compass_udp.$(OBJEXT) init.$(OBJEXT)
calibra_bussola_OBJECTS = $(am_calibra_bussola_OBJECTS)
calibra_bussola_LDADD = $(LDADD)
calibra_bussola_DEPENDENCIES =
am_calibra_hmc5883l_OBJECTS = calibra_hmc5883l.$(OBJEXT) \
	compass.$(OBJEXT) file_lock.$(OBJEXT) motors.$(OBJEXT) \
	hmc5883l_udp.$(OBJEXT) init.$(OBJEXT) gps_udp.$(OBJEXT) \
	i2c.$(OBJEXT) joystick.$(OBJEXT) pid.$(OBJEXT) gps.$(OBJEXT)
calibra_hmc5883l_OBJECTS = $(am_calibra_hmc5883l_OBJECTS)
calibra_hmc5883l_LDADD = $(LDADD)
calibra_hmc5883l_DEPENDENCIES =
am_compass_dump_OBJECTS = compass_dump.$(OBJEXT) compass.$(OBJEXT) \
	i2c.$(OBJEXT)
compass_dump_OBJECTS = $(am_compass_dump_OBJECTS)
compass_dump_LDADD = $(LDADD)
compass_dump_DEPENDENCIES =
am_hmc5883l_OBJECTS = hmc5883l_test.$(OBJEXT) hmc5883l.$(OBJEXT) \
	i2c.$(OBJEXT) file_lock.$(OBJEXT) compass.$(OBJEXT)
hmc5883l_OBJECTS = $(am_hmc5883l_OBJECTS)
hmc5883l_LDADD = $(LDADD)
hmc5883l_DEPENDENCIES =
am_hmc5883l_log_OBJECTS = hmc5883l_log.$(OBJEXT) hmc5883l.$(OBJEXT) \
	i2c.$(OBJEXT) file_lock.$(OBJEXT) compass.$(OBJEXT) \
	motors.$(OBJEXT)
hmc5883l_log_OBJECTS = $(am_hmc5883l_log_OBJECTS)
hmc5883l_log_LDADD = $(LDADD)
hmc5883l_log_DEPENDENCIES =
am_hmc_log_rotate_OBJECTS = hmc_log_rotate.$(OBJEXT) compass.$(OBJEXT) \
	file_lock.$(OBJEXT) motors.$(OBJEXT) hmc5883l_udp.$(OBJEXT) \
	init.$(OBJEXT) gps_udp.$(OBJEXT) i2c.$(OBJEXT) \
	joystick.$(OBJEXT)
hmc_log_rotate_OBJECTS = $(am_hmc_log_rotate_OBJECTS)
hmc_log_rotate_LDADD = $(LDADD)
hmc_log_rotate_DEPENDENCIES =
am_i2c_features_OBJECTS = i2c.$(OBJEXT) i2c_features.$(OBJEXT)
i2c_features_OBJECTS = $(am_i2c_features_OBJECTS)
i2c_features_LDADD = $(LDADD)
i2c_features_DEPENDENCIES =
am_joystick_kill_OBJECTS = joystick_kill.$(OBJEXT) joystick.$(OBJEXT)
joystick_kill_OBJECTS = $(am_joystick_kill_OBJECTS)
joystick_kill_LDADD = $(LDADD)
joystick_kill_DEPENDENCIES =
am_leds_control_OBJECTS = leds_control.$(OBJEXT) i2c.$(OBJEXT)
leds_control_OBJECTS = $(am_leds_control_OBJECTS)
leds_control_LDADD = $(LDADD)
leds_control_DEPENDENCIES =
am_motors_test_OBJECTS = motors_test.$(OBJEXT) motors.$(OBJEXT) \
	i2c.$(OBJEXT) file_lock.$(OBJEXT)
motors_test_OBJECTS = $(am_motors_test_OBJECTS)
motors_test_LDADD = $(LDADD)
motors_test_DEPENDENCIES =
am_motors_i2c_force_OBJECTS = motors_i2c_force.$(OBJEXT) \
	motors.$(OBJEXT) i2c.$(OBJEXT) file_lock.$(OBJEXT)
motors_i2c_force_OBJECTS = $(am_motors_i2c_force_OBJECTS)
motors_i2c_force_LDADD = $(LDADD)
motors_i2c_force_DEPENDENCIES =
am_motors_i2c_force_alternating_OBJECTS =  \
	motors_i2c_force_alternating.$(OBJEXT) motors.$(OBJEXT) \
	i2c.$(OBJEXT) file_lock.$(OBJEXT)
motors_i2c_force_alternating_OBJECTS =  \
	$(am_motors_i2c_force_alternating_OBJECTS)
motors_i2c_force_alternating_LDADD = $(LDADD)
motors_i2c_force_alternating_DEPENDENCIES =
am_nmea_OBJECTS = serial.$(OBJEXT) serial_nmea.$(OBJEXT)
nmea_OBJECTS = $(am_nmea_OBJECTS)
nmea_LDADD = $(LDADD)
nmea_DEPENDENCIES =
am_teste_bussola_OBJECTS = teste_bussola.$(OBJEXT) i2c.$(OBJEXT) \
	joystick.$(OBJEXT) compass.$(OBJEXT) file_lock.$(OBJEXT) \
	motors.$(OBJEXT) compass_udp.$(OBJEXT) init.$(OBJEXT)
teste_bussola_OBJECTS = $(am_teste_bussola_OBJECTS)
teste_bussola_LDADD = $(LDADD)
teste_bussola_DEPENDENCIES =
am_trekking_OBJECTS = main.$(OBJEXT) motors.$(OBJEXT) \
	file_lock.$(OBJEXT) joystick.$(OBJEXT) i2c.$(OBJEXT) \
	compass.$(OBJEXT) init.$(OBJEXT) hook.$(OBJEXT) \
	cont_array.$(OBJEXT)
trekking_OBJECTS = $(am_trekking_OBJECTS)
trekking_LDADD = $(LDADD)
trekking_DEPENDENCIES =
am_udp_recv_bussola_OBJECTS = udp_recv_bussola.$(OBJEXT)
udp_recv_bussola_OBJECTS = $(am_udp_recv_bussola_OBJECTS)
udp_recv_bussola_LDADD = $(LDADD)
udp_recv_bussola_DEPENDENCIES =
am_udp_recv_gps_OBJECTS = udp_recv_gps.$(OBJEXT)
udp_recv_gps_OBJECTS = $(am_udp_recv_gps_OBJECTS)
udp_recv_gps_LDADD = $(LDADD)
udp_recv_gps_DEPENDENCIES =
am_udp_recv_hmc5883l_OBJECTS = udp_recv_hmc5883l.$(OBJEXT)
udp_recv_hmc5883l_OBJECTS = $(am_udp_recv_hmc5883l_OBJECTS)
udp_recv_hmc5883l_LDADD = $(LDADD)
udp_recv_hmc5883l_DEPENDENCIES =
am_udp_recv_proximity_OBJECTS = udp_recv_proximity.$(OBJEXT)
udp_recv_proximity_OBJECTS = $(am_udp_recv_proximity_OBJECTS)
udp_recv_proximity_LDADD = $(LDADD)
udp_recv_proximity_DEPENDENCIES =
am_udp_send_OBJECTS = udp_send.$(OBJEXT)
udp_send_OBJECTS = $(am_udp_send_OBJECTS)
udp_send_LDADD = $(LDADD)
udp_send_DEPENDENCIES =
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
DEFAULT_INCLUDES = -I.
depcomp = $(SHELL) $(top_srcdir)/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC      " $@;
am__v_CC_1 = 
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD    " $@;
am__v_CCLD_1 = 
SOURCES = $(calibra_bussola_SOURCES) $(calibra_hmc5883l_SOURCES) \
	$(compass_dump_SOURCES) $(hmc5883l_SOURCES) \
	$(hmc5883l_log_SOURCES) $(hmc_log_rotate_SOURCES) \
	$(i2c_features_SOURCES) $(joystick_kill_SOURCES) \
	$(leds_control_SOURCES) $(motors_test_SOURCES) \
	$(motors_i2c_force_SOURCES) \
	$(motors_i2c_force_alternating_SOURCES) $(nmea_SOURCES) \
	$(teste_bussola_SOURCES) $(trekking_SOURCES) \
	$(udp_recv_bussola_SOURCES) $(udp_recv_gps_SOURCES) \
	$(udp_recv_hmc5883l_SOURCES) $(udp_recv_proximity_SOURCES) \
	$(udp_send_SOURCES)
DIST_SOURCES = $(calibra_bussola_SOURCES) $(calibra_hmc5883l_SOURCES) \
	$(compass_dump_SOURCES) $(hmc5883l_SOURCES) \
	$(hmc5883l_log_SOURCES) $(hmc_log_rotate_SOURCES) \
	$(i2c_features_SOURCES) $(joystick_kill_SOURCES) \
	$(leds_control_SOURCES) $(motors_test_SOURCES) \
	$(motors_i2c_force_SOURCES) \
	$(motors_i2c_force_alternating_SOURCES) $(nmea_SOURCES) \
	$(teste_bussola_SOURCES) $(trekking_SOURCES) \
	$(udp_recv_bussola_SOURCES) $(udp_recv_gps_SOURCES) \
	$(udp_recv_hmc5883l_SOURCES) $(udp_recv_proximity_SOURCES) \
	$(udp_send_SOURCES)
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) \
	$(LISP)config.h.in
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
CSCOPE = cscope
AM_RECURSIVE_TARGETS = cscope
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)
am__remove_distdir = \
  if test -d "$(distdir)"; then \
    find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
      && rm -rf "$(distdir)" \
      || { sleep 5 && rm -rf "$(distdir)"; }; \
  else :; fi
am__post_remove_distdir = $(am__remove_distdir)
DIST_ARCHIVES = $(distdir).tar.gz
GZIP_ENV = --best
DIST_TARGETS = dist-gzip
distuninstallcheck_listfiles = find . -type f -print
am__distuninstallcheck_listfiles = $(distuninstallcheck_listfiles) \
  | sed 's|^\./|$(prefix)/|' | grep -v '$(infodir)/dir$$'
distcleancheck_listfiles = find . -type f -print
ACLOCAL = aclocal-1.14
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 1
AUTOCONF = autoconf
AUTOHEADER = autoheader
AUTOMAKE = automake-1.14
AWK = gawk
CC = gcc
CCDEPMODE = depmode=gcc3
CFLAGS = 
CPPFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DEV = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EXEEXT = 
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LDFLAGS = 
LIBOBJS = 
LIBS = 
LTLIBOBJS = 
MAKEINFO = makeinfo
MKDIR_P = /usr/bin/mkdir -p
OBJEXT = o
PACKAGE = trekking
PACKAGE_BUGREPORT = tiago.shibata@gmail.com
PACKAGE_NAME = Trekking
PACKAGE_STRING = Trekking 0.1
PACKAGE_TARNAME = trekking
PACKAGE_URL = 
PACKAGE_VERSION = 0.1
PATH_SEPARATOR = :
SET_MAKE = 
SHELL = /bin/sh
STRIP = 
VERSION = 0.1
abs_builddir = /home/tiago/trekking-git
abs_srcdir = /home/tiago/trekking-git
abs_top_builddir = /home/tiago/trekking-git
abs_top_srcdir = /home/tiago/trekking-git
ac_ct_CC = gcc
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build_alias = 
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host_alias = 
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/tiago/trekking-git/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = $(MKDIR_P)
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = 
top_builddir = .
top_srcdir = .
trekking_SOURCES = main.c motors.c file_lock.c joystick.c i2c.c compass.c init.c hook.c cont_array.c
i2c_features_SOURCES = i2c.c i2c_features.c
compass_dump_SOURCES = compass_dump.c compass.c i2c.c
leds_control_SOURCES = leds_control.c i2c.c
calibra_bussola_SOURCES = calibra_bussola.c i2c.c joystick.c compass.c file_lock.c motors.c compass_udp.c init.c
calibra_hmc5883l_SOURCES = calibra_hmc5883l.c compass.c file_lock.c motors.c hmc5883l_udp.c init.c gps_udp.c i2c.c joystick.c pid.c gps.c
hmc_log_rotate_SOURCES = hmc_log_rotate.c compass.c file_lock.c motors.c hmc5883l_udp.c init.c gps_udp.c i2c.c joystick.c
nmea_SOURCES = serial.c serial_nmea.c
hmc5883l_SOURCES = hmc5883l_test.c hmc5883l.c i2c.c file_lock.c compass.c
hmc5883l_log_SOURCES = hmc5883l_log.c hmc5883l.c i2c.c file_lock.c compass.c motors.c
motors_test_SOURCES = motors_test.c motors.c i2c.c file_lock.c
udp_send_SOURCES = udp_send.c
udp_recv_bussola_SOURCES = udp_recv_bussola.c
udp_recv_hmc5883l_SOURCES = udp_recv_hmc5883l.c
udp_recv_gps_SOURCES = udp_recv_gps.c
udp_recv_proximity_SOURCES = udp_recv_proximity.c
motors_i2c_force_SOURCES = motors_i2c_force.c motors.c i2c.c file_lock.c
motors_i2c_force_alternating_SOURCES = motors_i2c_force_alternating.c motors.c i2c.c file_lock.c
teste_bussola_SOURCES = teste_bussola.c i2c.c joystick.c compass.c file_lock.c motors.c compass_udp.c init.c
joystick_kill_SOURCES = joystick_kill.c joystick.c
OPTIMIZE = -O0 -g -flto #-DDEBUG
LDADD = -lpthread -lm 
AM_CFLAGS = -Wall -Wextra -I$(srcdir)/include $(OPTIMIZE)
all: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-am

.SUFFIXES:
.SUFFIXES: .c .o .obj
am--refresh: Makefile
	@:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      echo ' cd $(srcdir) && $(AUTOMAKE) --foreign'; \
	      $(am__cd) $(srcdir) && $(AUTOMAKE) --foreign \
		&& exit 0; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --foreign Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --foreign Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    echo ' $(SHELL) ./config.status'; \
	    $(SHELL) ./config.status;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck

$(top_srcdir)/configure:  $(am__configure_deps)
	$(am__cd) $(srcdir) && $(AUTOCONF)
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	$(am__cd) $(srcdir) && $(ACLOCAL) $(ACLOCAL_AMFLAGS)
$(am__aclocal_m4_deps):

config.h: stamp-h1
	@test -f $@ || rm -f stamp-h1
	@test -f $@ || $(MAKE) $(AM_MAKEFLAGS) stamp-h1

stamp-h1: $(srcdir)/config.h.in $(top_builddir)/config.status
	@rm -f stamp-h1
	cd $(top_builddir) && $(SHELL) ./config.status config.h
$(srcdir)/config.h.in:  $(am__configure_deps) 
	($(am__cd) $(top_srcdir) && $(AUTOHEADER))
	rm -f stamp-h1
	touch $@

distclean-hdr:
	-rm -f config.h stamp-h1
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(bindir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(bindir)" || exit 1; \
	fi; \
	for p in $$list; do echo "$$p $$p"; done | \
	sed 's/$(EXEEXT)$$//' | \
	while read p p1; do if test -f $$p \
	  ; then echo "$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n;h' \
	    -e 's|.*|.|' \
	    -e 'p;x;s,.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/' | \
	sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1 } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) files[d] = files[d] " " $$1; \
	    else { print "f", $$3 "/" $$4, $$1; } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	    if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	    test -z "$$files" || { \
	      echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$files '$(DESTDIR)$(bindir)$$dir'"; \
	      $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$files "$(DESTDIR)$(bindir)$$dir" || exit $$?; \
	    } \
	; done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	files=`for p in $$list; do echo "$$p"; done | \
	  sed -e 'h;s,^.*/,,;s/$(EXEEXT)$$//;$(transform)' \
	      -e 's/$$/$(EXEEXT)/' \
	`; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(bindir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(bindir)" && rm -f $$files

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

calibra_bussola$(EXEEXT): $(calibra_bussola_OBJECTS) $(calibra_bussola_DEPENDENCIES) $(EXTRA_calibra_bussola_DEPENDENCIES) 
	@rm -f calibra_bussola$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(calibra_bussola_OBJECTS) $(calibra_bussola_LDADD) $(LIBS)

calibra_hmc5883l$(EXEEXT): $(calibra_hmc5883l_OBJECTS) $(calibra_hmc5883l_DEPENDENCIES) $(EXTRA_calibra_hmc5883l_DEPENDENCIES) 
	@rm -f calibra_hmc5883l$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(calibra_hmc5883l_OBJECTS) $(calibra_hmc5883l_LDADD) $(LIBS)

compass_dump$(EXEEXT): $(compass_dump_OBJECTS) $(compass_dump_DEPENDENCIES) $(EXTRA_compass_dump_DEPENDENCIES) 
	@rm -f compass_dump$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(compass_dump_OBJECTS) $(compass_dump_LDADD) $(LIBS)

hmc5883l$(EXEEXT): $(hmc5883l_OBJECTS) $(hmc5883l_DEPENDENCIES) $(EXTRA_hmc5883l_DEPENDENCIES) 
	@rm -f hmc5883l$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(hmc5883l_OBJECTS) $(hmc5883l_LDADD) $(LIBS)

hmc5883l-log$(EXEEXT): $(hmc5883l_log_OBJECTS) $(hmc5883l_log_DEPENDENCIES) $(EXTRA_hmc5883l_log_DEPENDENCIES) 
	@rm -f hmc5883l-log$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(hmc5883l_log_OBJECTS) $(hmc5883l_log_LDADD) $(LIBS)

hmc_log_rotate$(EXEEXT): $(hmc_log_rotate_OBJECTS) $(hmc_log_rotate_DEPENDENCIES) $(EXTRA_hmc_log_rotate_DEPENDENCIES) 
	@rm -f hmc_log_rotate$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(hmc_log_rotate_OBJECTS) $(hmc_log_rotate_LDADD) $(LIBS)

i2c_features$(EXEEXT): $(i2c_features_OBJECTS) $(i2c_features_DEPENDENCIES) $(EXTRA_i2c_features_DEPENDENCIES) 
	@rm -f i2c_features$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(i2c_features_OBJECTS) $(i2c_features_LDADD) $(LIBS)

joystick_kill$(EXEEXT): $(joystick_kill_OBJECTS) $(joystick_kill_DEPENDENCIES) $(EXTRA_joystick_kill_DEPENDENCIES) 
	@rm -f joystick_kill$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(joystick_kill_OBJECTS) $(joystick_kill_LDADD) $(LIBS)

leds_control$(EXEEXT): $(leds_control_OBJECTS) $(leds_control_DEPENDENCIES) $(EXTRA_leds_control_DEPENDENCIES) 
	@rm -f leds_control$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(leds_control_OBJECTS) $(leds_control_LDADD) $(LIBS)

motors-test$(EXEEXT): $(motors_test_OBJECTS) $(motors_test_DEPENDENCIES) $(EXTRA_motors_test_DEPENDENCIES) 
	@rm -f motors-test$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(motors_test_OBJECTS) $(motors_test_LDADD) $(LIBS)

motors_i2c_force$(EXEEXT): $(motors_i2c_force_OBJECTS) $(motors_i2c_force_DEPENDENCIES) $(EXTRA_motors_i2c_force_DEPENDENCIES) 
	@rm -f motors_i2c_force$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(motors_i2c_force_OBJECTS) $(motors_i2c_force_LDADD) $(LIBS)

motors_i2c_force_alternating$(EXEEXT): $(motors_i2c_force_alternating_OBJECTS) $(motors_i2c_force_alternating_DEPENDENCIES) $(EXTRA_motors_i2c_force_alternating_DEPENDENCIES) 
	@rm -f motors_i2c_force_alternating$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(motors_i2c_force_alternating_OBJECTS) $(motors_i2c_force_alternating_LDADD) $(LIBS)

nmea$(EXEEXT): $(nmea_OBJECTS) $(nmea_DEPENDENCIES) $(EXTRA_nmea_DEPENDENCIES) 
	@rm -f nmea$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(nmea_OBJECTS) $(nmea_LDADD) $(LIBS)

teste_bussola$(EXEEXT): $(teste_bussola_OBJECTS) $(teste_bussola_DEPENDENCIES) $(EXTRA_teste_bussola_DEPENDENCIES) 
	@rm -f teste_bussola$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(teste_bussola_OBJECTS) $(teste_bussola_LDADD) $(LIBS)

trekking$(EXEEXT): $(trekking_OBJECTS) $(trekking_DEPENDENCIES) $(EXTRA_trekking_DEPENDENCIES) 
	@rm -f trekking$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(trekking_OBJECTS) $(trekking_LDADD) $(LIBS)

udp_recv_bussola$(EXEEXT): $(udp_recv_bussola_OBJECTS) $(udp_recv_bussola_DEPENDENCIES) $(EXTRA_udp_recv_bussola_DEPENDENCIES) 
	@rm -f udp_recv_bussola$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(udp_recv_bussola_OBJECTS) $(udp_recv_bussola_LDADD) $(LIBS)

udp_recv_gps$(EXEEXT): $(udp_recv_gps_OBJECTS) $(udp_recv_gps_DEPENDENCIES) $(EXTRA_udp_recv_gps_DEPENDENCIES) 
	@rm -f udp_recv_gps$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(udp_recv_gps_OBJECTS) $(udp_recv_gps_LDADD) $(LIBS)

udp_recv_hmc5883l$(EXEEXT): $(udp_recv_hmc5883l_OBJECTS) $(udp_recv_hmc5883l_DEPENDENCIES) $(EXTRA_udp_recv_hmc5883l_DEPENDENCIES) 
	@rm -f udp_recv_hmc5883l$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(udp_recv_hmc5883l_OBJECTS) $(udp_recv_hmc5883l_LDADD) $(LIBS)

udp_recv_proximity$(EXEEXT): $(udp_recv_proximity_OBJECTS) $(udp_recv_proximity_DEPENDENCIES) $(EXTRA_udp_recv_proximity_DEPENDENCIES) 
	@rm -f udp_recv_proximity$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(udp_recv_proximity_OBJECTS) $(udp_recv_proximity_LDADD) $(LIBS)

udp_send$(EXEEXT): $(udp_send_OBJECTS) $(udp_send_DEPENDENCIES) $(EXTRA_udp_send_DEPENDENCIES) 
	@rm -f udp_send$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(udp_send_OBJECTS) $(udp_send_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/calibra_bussola.Po
include ./$(DEPDIR)/calibra_hmc5883l.Po
include ./$(DEPDIR)/compass.Po
include ./$(DEPDIR)/compass_dump.Po
include ./$(DEPDIR)/compass_udp.Po
include ./$(DEPDIR)/cont_array.Po
include ./$(DEPDIR)/file_lock.Po
include ./$(DEPDIR)/gps.Po
include ./$(DEPDIR)/gps_udp.Po
include ./$(DEPDIR)/hmc5883l.Po
include ./$(DEPDIR)/hmc5883l_log.Po
include ./$(DEPDIR)/hmc5883l_test.Po
include ./$(DEPDIR)/hmc5883l_udp.Po
include ./$(DEPDIR)/hmc_log_rotate.Po
include ./$(DEPDIR)/hook.Po
include ./$(DEPDIR)/i2c.Po
include ./$(DEPDIR)/i2c_features.Po
include ./$(DEPDIR)/init.Po
include ./$(DEPDIR)/joystick.Po
include ./$(DEPDIR)/joystick_kill.Po
include ./$(DEPDIR)/leds_control.Po
include ./$(DEPDIR)/main.Po
include ./$(DEPDIR)/motors.Po
include ./$(DEPDIR)/motors_i2c_force.Po
include ./$(DEPDIR)/motors_i2c_force_alternating.Po
include ./$(DEPDIR)/motors_test.Po
include ./$(DEPDIR)/pid.Po
include ./$(DEPDIR)/serial.Po
include ./$(DEPDIR)/serial_nmea.Po
include ./$(DEPDIR)/teste_bussola.Po
include ./$(DEPDIR)/udp_recv_bussola.Po
include ./$(DEPDIR)/udp_recv_gps.Po
include ./$(DEPDIR)/udp_recv_hmc5883l.Po
include ./$(DEPDIR)/udp_recv_proximity.Po
include ./$(DEPDIR)/udp_send.Po

.c.o:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ $<

.c.obj:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-am
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-am

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscope: cscope.files
	test ! -s cscope.files \
	  || $(CSCOPE) -b -q $(AM_CSCOPEFLAGS) $(CSCOPEFLAGS) -i cscope.files $(CSCOPE_ARGS)
clean-cscope:
	-rm -f cscope.files
cscope.files: clean-cscope cscopelist
cscopelist: cscopelist-am

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags
	-rm -f cscope.out cscope.in.out cscope.po.out cscope.files

distdir: $(DISTFILES)
	$(am__remove_distdir)
	test -d "$(distdir)" || mkdir "$(distdir)"
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
	-test -n "$(am__skip_mode_fix)" \
	|| find "$(distdir)" -type d ! -perm -755 \
		-exec chmod u+rwx,go+rx {} \; -o \
	  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -444 -exec $(install_sh) -c -m a+r {} {} \; \
	|| chmod -R a+r "$(distdir)"
dist-gzip: distdir
	tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__post_remove_distdir)

dist-bzip2: distdir
	tardir=$(distdir) && $(am__tar) | BZIP2=$${BZIP2--9} bzip2 -c >$(distdir).tar.bz2
	$(am__post_remove_distdir)

dist-lzip: distdir
	tardir=$(distdir) && $(am__tar) | lzip -c $${LZIP_OPT--9} >$(distdir).tar.lz
	$(am__post_remove_distdir)

dist-xz: distdir
	tardir=$(distdir) && $(am__tar) | XZ_OPT=$${XZ_OPT--e} xz -c >$(distdir).tar.xz
	$(am__post_remove_distdir)

dist-tarZ: distdir
	@echo WARNING: "Support for shar distribution archives is" \
	               "deprecated." >&2
	@echo WARNING: "It will be removed altogether in Automake 2.0" >&2
	tardir=$(distdir) && $(am__tar) | compress -c >$(distdir).tar.Z
	$(am__post_remove_distdir)

dist-shar: distdir
	@echo WARNING: "Support for distribution archives compressed with" \
		       "legacy program 'compress' is deprecated." >&2
	@echo WARNING: "It will be removed altogether in Automake 2.0" >&2
	shar $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).shar.gz
	$(am__post_remove_distdir)

dist-zip: distdir
	-rm -f $(distdir).zip
	zip -rq $(distdir).zip $(distdir)
	$(am__post_remove_distdir)

dist dist-all:
	$(MAKE) $(AM_MAKEFLAGS) $(DIST_TARGETS) am__post_remove_distdir='@:'
	$(am__post_remove_distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	case '$(DIST_ARCHIVES)' in \
	*.tar.gz*) \
	  GZIP=$(GZIP_ENV) gzip -dc $(distdir).tar.gz | $(am__untar) ;;\
	*.tar.bz2*) \
	  bzip2 -dc $(distdir).tar.bz2 | $(am__untar) ;;\
	*.tar.lz*) \
	  lzip -dc $(distdir).tar.lz | $(am__untar) ;;\
	*.tar.xz*) \
	  xz -dc $(distdir).tar.xz | $(am__untar) ;;\
	*.tar.Z*) \
	  uncompress -c $(distdir).tar.Z | $(am__untar) ;;\
	*.shar.gz*) \
	  GZIP=$(GZIP_ENV) gzip -dc $(distdir).shar.gz | unshar ;;\
	*.zip*) \
	  unzip $(distdir).zip ;;\
	esac
	chmod -R a-w $(distdir)
	chmod u+w $(distdir)
	mkdir $(distdir)/_build $(distdir)/_inst
	chmod a-w $(distdir)
	test -d $(distdir)/_build || exit 0; \
	dc_install_base=`$(am__cd) $(distdir)/_inst && pwd | sed -e 's,^[^:\\/]:[\\/],/,'` \
	  && dc_destdir="$${TMPDIR-/tmp}/am-dc-$$$$/" \
	  && am__cwd=`pwd` \
	  && $(am__cd) $(distdir)/_build \
	  && ../configure \
	    $(AM_DISTCHECK_CONFIGURE_FLAGS) \
	    $(DISTCHECK_CONFIGURE_FLAGS) \
	    --srcdir=.. --prefix="$$dc_install_base" \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && $(MAKE) $(AM_MAKEFLAGS) distuninstallcheck_dir="$$dc_install_base" \
	        distuninstallcheck \
	  && chmod -R a-w "$$dc_install_base" \
	  && ({ \
	       (cd ../.. && umask 077 && mkdir "$$dc_destdir") \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" install \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" uninstall \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" \
	            distuninstallcheck_dir="$$dc_destdir" distuninstallcheck; \
	      } || { rm -rf "$$dc_destdir"; exit 1; }) \
	  && rm -rf "$$dc_destdir" \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && rm -rf $(DIST_ARCHIVES) \
	  && $(MAKE) $(AM_MAKEFLAGS) distcleancheck \
	  && cd "$$am__cwd" \
	  || exit 1
	$(am__post_remove_distdir)
	@(echo "$(distdir) archives ready for distribution: "; \
	  list='$(DIST_ARCHIVES)'; for i in $$list; do echo $$i; done) | \
	  sed -e 1h -e 1s/./=/g -e 1p -e 1x -e '$$p' -e '$$x'
distuninstallcheck:
	@test -n '$(distuninstallcheck_dir)' || { \
	  echo 'ERROR: trying to run $@ with an empty' \
	       '$$(distuninstallcheck_dir)' >&2; \
	  exit 1; \
	}; \
	$(am__cd) '$(distuninstallcheck_dir)' || { \
	  echo 'ERROR: cannot chdir into $(distuninstallcheck_dir)' >&2; \
	  exit 1; \
	}; \
	test `$(am__distuninstallcheck_listfiles) | wc -l` -eq 0 \
	   || { echo "ERROR: files left after uninstall:" ; \
	        if test -n "$(DESTDIR)"; then \
	          echo "  (check DESTDIR support)"; \
	        fi ; \
	        $(distuninstallcheck_listfiles) ; \
	        exit 1; } >&2
distcleancheck: distclean
	@if test '$(srcdir)' = . ; then \
	  echo "ERROR: distcleancheck can only run from a VPATH build" ; \
	  exit 1 ; \
	fi
	@test `$(distcleancheck_listfiles) | wc -l` -eq 0 \
	  || { echo "ERROR: files left in build directory after distclean:" ; \
	       $(distcleancheck_listfiles) ; \
	       exit 1; } >&2
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS) config.h
installdirs:
	for dir in "$(DESTDIR)$(bindir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-binPROGRAMS clean-generic mostlyclean-am

distclean: distclean-am
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-hdr distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am: install-binPROGRAMS

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -rf $(top_srcdir)/autom4te.cache
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am: uninstall-binPROGRAMS

.MAKE: all install-am install-strip

.PHONY: CTAGS GTAGS TAGS all all-am am--refresh check check-am clean \
	clean-binPROGRAMS clean-cscope clean-generic cscope \
	cscopelist-am ctags ctags-am dist dist-all dist-bzip2 \
	dist-gzip dist-lzip dist-shar dist-tarZ dist-xz dist-zip \
	distcheck distclean distclean-compile distclean-generic \
	distclean-hdr distclean-tags distcleancheck distdir \
	distuninstallcheck dvi dvi-am html html-am info info-am \
	install install-am install-binPROGRAMS install-data \
	install-data-am install-dvi install-dvi-am install-exec \
	install-exec-am install-html install-html-am install-info \
	install-info-am install-man install-pdf install-pdf-am \
	install-ps install-ps-am install-strip installcheck \
	installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic pdf pdf-am ps ps-am tags tags-am uninstall \
	uninstall-am uninstall-binPROGRAMS


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
