/*===-- analysis_ocaml.c - LLVM OCaml Glue ----------------------*- C++ -*-===*\
|*                                                                            *|
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM          *|
|* Exceptions.                                                                *|
|* See https://llvm.org/LICENSE.txt for license information.                  *|
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception                    *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This file glues LLVM's OCaml interface to its C interface. These functions *|
|* are by and large transparent wrappers to the corresponding C functions.    *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#include "llvm-c/Analysis.h"
#include "llvm-c/Core.h"
#include "caml/alloc.h"
#include "caml/mlvalues.h"
#include "caml/memory.h"
#include "llvm_ocaml.h"

/* Llvm.llmodule -> string option */
value llvm_verify_module(value M) {
  CAMLparam1(M);
  CAMLlocal2(String, Option);

  char *Message;
  int Result =
      LLVMVerifyModule(Module_val(M), LLVMReturnStatusAction, &Message);

  if (0 == Result) {
    Option = Val_none;
  } else {
    String = caml_copy_string(Message);
    Option = caml_alloc_some(String);
  }

  LLVMDisposeMessage(Message);

  CAMLreturn(Option);
}

/* Llvm.llvalue -> bool */
value llvm_verify_function(value Fn) {
  CAMLparam1(Fn);
  CAMLreturn(
      Val_bool(LLVMVerifyFunction(Value_val(Fn), LLVMReturnStatusAction) == 0));
}

/* Llvm.llmodule -> unit */
value llvm_assert_valid_module(value M) {
  CAMLparam1(M);
  LLVMVerifyModule(Module_val(M), LLVMAbortProcessAction, 0);
  CAMLreturn(Val_unit);
}

/* Llvm.llvalue -> unit */
value llvm_assert_valid_function(value Fn) {
  CAMLparam1(Fn);
  LLVMVerifyFunction(Value_val(Fn), LLVMAbortProcessAction);
  CAMLreturn(Val_unit);
}

/* Llvm.llvalue -> unit */
value llvm_view_function_cfg(value Fn) {
  CAMLparam1(Fn);
  LLVMViewFunctionCFG(Value_val(Fn));
  CAMLreturn(Val_unit);
}

/* Llvm.llvalue -> unit */
value llvm_view_function_cfg_only(value Fn) {
  CAMLparam1(Fn);
  LLVMViewFunctionCFGOnly(Value_val(Fn));
  CAMLreturn(Val_unit);
}
