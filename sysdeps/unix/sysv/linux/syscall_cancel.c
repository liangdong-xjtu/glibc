/* Default cancellation syscall bridge.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>
#include <pthreadP.h>

#define ADD_LABEL(__label)		\
  asm volatile (			\
    ".global " __label "\t\n"		\
    ".type " __label ",@function\t\n" 	\
    __label ":\n");

/* This is the generic version of the cancellable syscall code which
   basically adds the label guards (__syscall_cancel_arch_{start,end}) used
   on sigcancel_handler (nptl-init.c) to check if the cancelled syscalls
   have side-effects that need to be signel to program.

   An important contrainst should be observed when using this generic
   implementation: the __syscall_cancel_arch_end should point to the
   immediate next instruction after the syscall one.  This is because
   will signal interrupted syscall with side effects by setting the
   signal frame PC right after the syscall instruction.

   If the INTERNAL_SYSCALL_NCS macro use more instruction to get the
   erro condition from kernel (from instance on powerpc), one should
   either adjust the macro or provide a custom implementation.   */
long int
__syscall_cancel_arch (volatile int *ch, __syscall_arg_t nr,
		       __syscall_arg_t a1, __syscall_arg_t a2,
		       __syscall_arg_t a3, __syscall_arg_t a4,
		       __syscall_arg_t a5, __syscall_arg_t a6)
{
  ADD_LABEL ("__syscall_cancel_arch_start");
  if (__glibc_unlikely (*ch & CANCELED_BITMASK))
    __syscall_do_cancel();

  INTERNAL_SYSCALL_DECL(err);
  long int result = INTERNAL_SYSCALL_NCS (nr, err, 6, a1, a2, a3, a4, a5, a6);
  ADD_LABEL ("__syscall_cancel_arch_end");
  if (INTERNAL_SYSCALL_ERROR_P (result, err))
    return -INTERNAL_SYSCALL_ERRNO (result, err);
  return result;
}
libc_hidden_def (__syscall_cancel_arch)
