GLIBC_PROVIDES dnl See aclocal.m4 in the top level source directory.
# Local configure fragment for sysdeps/unix/sysv/linux/x86.

if test x"$enable_cet" = xdefault || test x"$enable_cet" = xyes; then
  # See whether CET is default.
  AC_CACHE_CHECK(whether CET is default,
		 libc_cv_x86_cet_default, [dnl
cat > conftest.c <<EOF
#if !defined __IBT__ || !defined __SHSTK__
# error CET isn't enabled.
#endif
EOF
		 if AC_TRY_COMMAND(${CC-cc} -S $CFLAGS conftest.c -o conftest.s 1>&AS_MESSAGE_LOG_FD); then
		   libc_cv_x86_cet_default=yes
		 else
		   libc_cv_x86_cet_default=no
		 fi
		 rm -rf conftest*])
  if test $libc_cv_x86_cet_default = yes; then
    if test x"$enable_cet" = xdefault; then
      enable_cet=yes
    fi
  else
    if test x"$enable_cet" = xdefault; then
      enable_cet=no
    else
      AC_MSG_ERROR([$CC doesn't default to CET])
    fi
  fi
fi
if test $enable_cet = yes; then
  # Check if assembler supports CET.
  AC_CACHE_CHECK(whether $AS supports CET,
		 libc_cv_x86_cet_as, [dnl
cat > conftest.s <<EOF
	incsspd %ecx
EOF
		 if AC_TRY_COMMAND(${CC-cc} -c $CFLAGS conftest.s -o conftest.o 1>&AS_MESSAGE_LOG_FD); then
		   libc_cv_x86_cet_as=yes
		 else
		   libc_cv_x86_cet_as=no
		 fi
		 rm -rf conftest*])
  if test $libc_cv_x86_cet_as = no; then
    AC_MSG_ERROR([$AS doesn't support CET])
  fi
  AC_DEFINE(ENABLE_CET, 1,
	    [Enable Intel Control-flow Enforcement Technology (CET)])
fi
LIBC_CONFIG_VAR([enable-cet], [$enable_cet])
