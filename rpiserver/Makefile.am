##
## @info   rpiserver ver.1.0
## @author Vladimir Roncevic <elektron.ronca@gmail.com>
## @brief  Process this file with automake to produce Makefile.in
##

# Sub directories with sources
SUBDIRS = src

# Documentation files
dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

EXTRA_DIST = \
	autogen.sh

uninstall-local:
	-rm -rf $(docdir)
