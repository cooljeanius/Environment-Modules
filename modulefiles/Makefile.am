###########################################################################
##
## File:        modulefiles/Makefile.am
## Versions:    $Id: af42d2fb4a611eb35a5beb7537ff358503c5a2a4 $
## Created:     2002/06/14
##
###########################################################################


AUTOMAKE_OPTIONS = foreign no-installinfo -Wall

BASEMODULES = \
	HOME dot module-cvs module-git module-info modules null use.own \
	notavail

# add any new languages for the lang/ modulefiles here
# and in the lang/.mklang script
LANGUAGES= \
	lang/de lang/en lang/es lang/fi lang/fr lang/gr			\
	lang/it lang/ja lang/no lang/ru lang/us lang/zh

ALLMODULES=${BASEMODULES} ${LANGUAGES} lang/.mklang

EXTRA_SCRIPTS = ${BASEMODULES} version

DISTCLEANFILES = .DS_Store

all:	${EXTRA_SCRIPTS}

${LANGUAGES} : lang/.mklang
	-(cd lang; ./.mklang)

install-data-local : install-basemodules install-version

install-basemodules : $(BASEMODULES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(prefix)/modulefiles
	@list='$(BASEMODULES)'; for p in $$list; do \
	  if test -f $$p \
	  ; then \
	   echo " $(INSTALL_DATA_ENV) $(INSTALL_DATA) $$p $(DESTDIR)$(prefix)/modulefiles/$$p"; \
	   $(INSTALL_DATA_ENV) $(INSTALL_DATA) $$p $(DESTDIR)$(prefix)/modulefiles/$$p; \
	  else :; fi; \
	done

@NOTVERSIONING@install-version : version
@VERSIONING@install-version-not :
	@echo Not installing version file

@VERSIONING@install-version : version
@NOTVERSIONING@install-version-not :
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(VERSIONPATH)
	@if test x$(WANTS_VERSIONING) = x1 ; then \
	  echo " $(INSTALL_DATA_ENV) $(INSTALL_DATA) version $(DESTDIR)$(VERSIONPATH)/$(VERSION)"; \
	  $(INSTALL_DATA_ENV) $(INSTALL_DATA) version $(DESTDIR)$(VERSIONPATH)/$(VERSION) ; \
	fi

uninstall-local : uninstall-basemodules uninstall-version

uninstall-basemodules :
	@$(NORMAL_UNINSTALL)
	@if test x$(WANTS_VERSIONING) = x1 ; then \
	  list='$(BASEMODULES)'; for p in $$list; do \
	    echo " rm -f $(DESTDIR)$(prefix)/modulefiles/$$p"; \
	    rm -f $(DESTDIR)$(prefix)/modulefiles/$$p; \
	  done; \
	else \
	  echo "*** BASE MODULES are not uninstalled from $(DESTDIR)$(prefix)/modulefiles ***"; \
	  echo "*** Do This Yourself If you really want to ***"; \
	fi

@NOTVERSIONING@uninstall-version :

@VERSIONING@uninstall-version :
@NOTVERSIONING@uninstall-version-not :
	@$(NORMAL_UNINSTALL)
	@if test x$(WANTS_VERSIONING) = x1 ; then \
	    echo " rm -rf $(DESTDIR)$(VERSIONPATH)/$(VERSION)" ; \
	    rm -rf $(DESTDIR)$(VERSIONPATH)/$(VERSION) ; \
	else \
	  echo "*** VERSION modulefile $(DESTDIR)$(VERSIONPATH)/$(VERSION) not uninstalled ***"; \
	  echo "*** Do This Yourself If you really want to ***"; \
	fi

regen_dot : dot.in ../config.status
	(cd ..; config.status modulefiles/dot)
.PHONY: regen_dot

regen_module-cvs : module-cvs.in ../config.status
	(cd ..; config.status modulefiles/module-cvs)
.PHONY: regen_module-cvs

regen_module-git : module-git.in ../config.status
	(cd ..; config.status modulefiles/module-git)
.PHONY: regen_module-git

regen_module-info : module-info.in ../config.status
	(cd ..; config.status modulefiles/module-info)
.PHONY: regen_module-info

regen_modules : modules.in ../config.status
	(cd ..; config.status modulefiles/modules)
.PHONY: regen_modules

regen_null : null.in ../config.status
	(cd ..; config.status modulefiles/null)
.PHONY: regen_null

regen_use.own : use.own.in ../config.status
	(cd ..; config.status modulefiles/use.own)
.PHONY: regen_use.own

regen_version : version.in ../config.status
	(cd ..; config.status modulefiles/version)
.PHONY: regen_version

check :

clean-local :
	-rm -f ${BASEMODULES}
	-(cd lang; ./.mklang clean)
