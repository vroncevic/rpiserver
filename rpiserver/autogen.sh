#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

DIE=0

(test -f $srcdir/configure.ac) || {
    echo "**Error**: Directory \`$srcdir' does not look like the top-level package directory"
    exit 1
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed."
  echo "Download the appropriate package for your distribution."
  DIE=1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`automake' installed."
  echo "Download the appropriate package for your distribution."
  DIE=1
  NO_AUTOMAKE=yes
}

# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'."
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

if test -z "$*"; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the \`$0' command line."
  echo
fi

for coin in `find $srcdir -path $srcdir/CVS -prune -o -name configure.ac -print`
do 
  dr=`dirname $coin`
  if test -f $dr/NO-AUTO-GEN; then
    echo "skipping $dr -- flagged as no auto-gen"
  else
    echo "processing $dr"
    ( cd $dr
      aclocalinclude="$ACLOCAL_FLAGS"
      echo "Running aclocal $aclocalinclude ..."
      aclocal $aclocalinclude
      if grep "^A[CM]_CONFIG_HEADER" configure.ac >/dev/null; then
        echo "Running autoheader..."
        autoheader
      fi
      echo "Running automake --gnu --add-missing --copy ..."
      automake --add-missing --copy --gnu
      echo "Running autoconf ..."
      autoconf
    )
  fi
done

if test x$NOCONFIGURE = x; then
  echo "Running $srcdir/configure $@ ..."
  $srcdir/configure "$@" \
  && echo "Now type \`make' to compile." || exit 1
else
  echo "Skipping configure process."
fi
