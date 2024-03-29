###########################################################################
##
## File:        ./Makefile.am
## Versions:    $Id: 4b2c7f8d94dbf93e345b20a32c39a10996a06e43 $
## Created:     2002/06/14
##
###########################################################################
##
## run  automake to generate a Makefile.in:
##	Makefile.am \
##	configure.ac >  Makefile.in
##	aclocal.m4  /
##

ACLOCAL_AMFLAGS = -I m4 --install
MY_WARNFLAGS = -Wall -Wimplicit -Wmissing-prototypes -Wreturn-type \
-Wmissing-braces -Wparentheses -Wswitch -Wunused-parameter \
-Wunused-variable -Wunused-value -Wunknown-pragmas -Wshadow \
-Wfour-char-constants -Wsign-compare -Wnewline-eof -Wconversion \
-Wshorten-64-to-32 -Wno-unused-label -Wno-expansion-to-defined
## CWARNFLAGS is subst-ed by the "XORG_CWARNFLAGS" macro in configure.ac:
AM_WARNFLAGS = $(CWARNFLAGS)
AM_CFLAGS = $(AM_WARNFLAGS)

man_MANS = doc/module.1 doc/modulefile.4
SUBDIRS = . init modulefiles @EXTDIR@ doc po
DIST_SUBDIRS = . init modulefiles etc ext doc
SOURCES_H = modules_def.h getopt.h config.h gettext.h
DISTCLEANFILES = *~ .DS_Store config/*~ m4/*~ m4/.DS_Store po/*~ autoscan.log
MAINTAINERCLEANFILES = aminclude_static.am configure.scan
EXTRA_DIST = config/config.rpath config/mkinstalldirs	\
	modules_def.h getopt.h uvec.h ovec.h testsuite INSTALL.RH7x	\
	init/filter init/bash.in init/sh.in init/zsh.in			\
	init/csh.in init/ksh.in init/perl.pm.in init/python.py.in	\
	init/ruby.rb.in init/cmake.in init/bash_completion.in		\
	modulefiles/lang/.mklang					\
	LICENSE.GPL ABOUT-NLS ChangeLog

bin_PROGRAMS = modulecmd
bin_SCRIPTS = etc/mkroot etc/add.modules ext/add.ext
## BASE_CFLAGS is subst-ed by the "XORG_COMPILER_FLAGS" macro called by the
## "XORG_CWARNFLAGS" macro in configure.ac:
modulecmd_CFLAGS=							\
	-DSYSCONFDIR=\"$(sysconfdir)\"					\
	-DBASEPREFIX=\"$(BASEPREFIX)\"					\
	-DMODULEPATH=\"$(MODULEPATH)\"					\
	-DVERSIONPATH=\"$(VERSIONPATH)\"				\
	-DINITPATH=\"$(INITPATH)\"					\
	@TCLX_INCLUDE_SPEC@ @TCL_INCLUDE_SPEC@ @X_CFLAGS@ $(BASE_CFLAGS)
modulecmd_LDADD= @TCLX_LIB_SPEC@ @TCL_LIB_SPEC@ @X_LIBS@ @X_EXTRA_LIBS@ @LIBS@
modulecmd_SOURCES = \
	ModuleCmd_Avail.c \
        ModuleCmd_Clear.c ModuleCmd_Display.c ModuleCmd_Help.c \
	ModuleCmd_Init.c ModuleCmd_List.c ModuleCmd_Load.c \
	ModuleCmd_Purge.c ModuleCmd_Switch.c ModuleCmd_Update.c \
	ModuleCmd_Use.c ModuleCmd_Whatis.c ModuleCmd_Refresh.c \
	cmdAlias.c cmdConflict.c cmdIsLoaded.c cmdInfo.c cmdMisc.c \
	cmdModule.c cmdPath.c cmdSetenv.c cmdUname.c cmdXResource.c \
	cmdUlvl.c cmdLog.c cmdChdir.c cmdTrace.c cmdVersion.c \
	cmdVerbose.c cmdWhatis.c init.c locate_module.c utility.c \
	utilmem.c main.c error.c getopt.c version.c uvec.c ovec.c

localedir = $(datadir)/locale

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

test: check-local

check-local:	modulecmd
	MODULEVERSION=@VERSION@; export MODULEVERSION; \
	TESTSUITEDIR=$(srcdir)/testsuite; export TESTSUITEDIR; \
	$(RUNTEST) --srcdir $$TESTSUITEDIR \
	$(AM_RUNTESTFLAGS) $(RUNTESTFLAGS) --tool=modules
	test -x "`which submitdiaginfo`" || pwd

ctags-local: CTAGS
CTAGS-local: $(modulecmd_SOURCES)
	$(CTAGS) $(modulecmd_SOURCES) $(SOURCES_H)

cxref:
	$(MAKE) clean
# may need to edit the cxref-cc script setting
	$(MAKE) CC=cxref-cc CXREFCC=$(CC)
	@echo "==============================="
	@echo "You may need to run this twice!"
	@echo "==============================="

SED=@SED@
CVS=@CVS@
GZIP=gzip
BZIP2=bzip2
## TODO: consider subst-ing GZIP and BZIP2?

patch: $(srcdir)/configure
	@echo "Creating patch file -- do this after CVS tagging"
	${CVS} rdiff -c \
	 -r `${SED} -n -e 's/^.*OLD TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c` \
	 -r `${SED} -n -e 's/^.*NEW TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c` \
	modules | ${GZIP} -c \
	> modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff.gz
	${CVS} rdiff -c \
	 -r `${SED} -n -e 's/^.*OLD TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c` \
	 -r `${SED} -n -e 's/^.*NEW TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c` \
	modules | ${BZIP2} -c \
	> modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff.bz2

tag:
	@echo "CVS tagging - TAG = "\
	  `${SED} -n -e 's/^.*NEW TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c`
	${CVS} rtag -b -F \
	  `${SED} -n -e 's/^.*NEW TAG.*"\(.*\)".*$$/\1/p' ${srcdir}/version.c` \
	  modules

# ChangeLog from pre-git days has been renamed as "ChangeLog-pre-git"
ChangeLog: CL

CL: rmChangeLog ./gitlog2changelog.py
	@echo "Updating ChangeLog (git-style)"
	-@./gitlog2changelog.py

CL-cvs: rmChangeLog ./cvs2cl.pl
	@echo "Updating ChangeLog (cvs-style)"
	-@./cvs2cl.pl

rmChangeLog: NEWS
	-@rm -f ChangeLog

configure.scan: $(modulecmd_SOURCES)
	autoscan || touch configure.scan

lsm:
# sends off the modules.lsm file to update the Linux Software Map
# this should be done by the maintainer (R.K.Owen) only!
	mail -s add modules@kooz.sj.ca.us,lsm@execpc.com < modules.lsm

DISTNAME=$(distdir)

distclean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache

ftp-local: dist
# this should be done by the maintainer (R.K.Owen) only!
	@echo "placing $(DISTNAME).tar.{gz|bz2}"
	-@if [ -e $(DISTNAME).tar.gz ]; then \
		cp $(DISTNAME).tar.gz /u/ftp/pub/rkowen/modules; \
	else echo "Cannot find $(DISTNAME).tar.gz"; fi
	-@if [ -e $(DISTNAME).tar.bz2 ]; then \
		cp $(DISTNAME).tar.bz2 /u/ftp/pub/rkowen/modules; \
	else echo "Cannot find $(DISTNAME).tar.bz2"; fi
	-@XXX=modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff.gz ; \
	echo "placing $$XXX"; \
	if [ -e $$XXX ]; then \
		cp $$XXX /u/ftp/pub/rkowen/modules; \
	else echo "Cannot find $$XXX"; fi
	-@XXX=modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff.bz2 ; \
	echo "placing $$XXX"; \
	if [ -e $$XXX ]; then \
		cp $$XXX /u/ftp/pub/rkowen/modules; \
	else echo "Cannot find $$XXX"; fi
	-@echo "placing modules.lsm"
	-@if [ -e modules.lsm ]; then \
		cp modules.lsm /u/ftp/pub/rkowen/modules; \
	else echo "Cannot find modules.lsm"; fi

dist-hook: CL
	-rm -rf `find $(distdir) -name CVS`

ftp: ftp-ibiblio ftp-sourceforge

ftp-ibiblio: dist
# do local files first ... mostly to verify their existence
# this should be done by the maintainer (R.K.Owen) only!
	@echo "placing ftp.ibiblio.org files";
	-@XXX=modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff."*" ; \
	echo "placing modules.lsm $$XXX $(DISTNAME).tar.*"; \
	./.ftp ftp.ibiblio.org /incoming/linux \
		modules.lsm $$XXX $(DISTNAME).tar.*;

ftp-sourceforge: dist
	@echo "placing modules.sourceforge.net files";
	-@XXX=modules-`${SED} -n -e 's/^.*OLD TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`-`${SED} -n -e 's/^.*NEW TAG.*"modules-\([0-9]*\)-\([0-9]*\)-\([0-9]*\)".*$$/\1.\2.\3/p' ${srcdir}/version.c`.diff."*" ; \
	echo "placing modules.lsm $$XXX $(DISTNAME).tar."*""; \
	scp modules.lsm $$XXX $(DISTNAME).tar.* rkowen@shell.sourceforge.net:ftp
	./.ftp upload.sourceforge.net /incoming \
		modules.lsm $$XXX $(DISTNAME).tar.*;

help:
	@echo ""
	@echo "make           - compiles sources to executable"
	@echo "make all       - same as above"
	@echo "make check     - runs test suite - needs dejagnu runtest"
	@echo "make install   - copy program & man pages to destination"
	@echo "make clean     - cleans out most useless files"
	@echo "make disthelp  - extra help for developers"
	@echo ""

disthelp:
	@echo "make ctags     - creates the vi ctags file"
	@echo "make cxref     - makes the xref HTML files - needs cxref"
	@echo "make dist      - creates a distribution tarballs"
	@echo "make distclean - cleans & removes most made files"
	@echo "make maintainer-clean - cleans even more"
	@echo "=============== requires git access ======================="
	@echo "make CL        - remakes the ChangeLog from the git commits"
	@echo "=============== maintainers only =========================="
	@echo "make lsm       - send off to the Linux Software Map"
	@echo "make ftp-local - upload tarball (good test)"
	@echo "make ftp-ibiblio      - upload to ibiblio"
	@echo "make ftp-sourceforge  - upload to sourceforge"
	@echo "make ftp       - upload to both ibiblio & sourceforge"
	@echo ""

