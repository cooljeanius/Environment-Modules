###########################################################################
##
## File:        etc/Makefile.am
## Versions:    $Id: 997b2f71736ed56895009e0cc2857c8380de6594 $
## Created:     2002/06/14
##
###########################################################################


AUTOMAKE_OPTIONS = foreign dejagnu no-installinfo -Wall

EXTRA_scripts = add.modules mkroot

DISTCLEANFILES = .DS_Store

install-data-local : $(EXTRA_scripts)
	$(mkinstalldirs) $(DESTDIR)$(prefix)/bin
	@list='$(EXTRA_scripts)'; for p in $$list; do \
	  if test -f $$p \
	  ; then \
	   echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(prefix)/bin/$$p"; \
	   $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(prefix)/bin/$$p; \
	  else :; fi; \
	done

uninstall-local :
	@list='$(EXTRA_scripts)'; for p in $$list; do \
	  echo " rm -f $(DESTDIR)$(prefix)/bin/$$p"; \
	  rm -f $(DESTDIR)$(prefix)/bin/$$p; \
	done

check :

clean-local :
