; ModuleID = 'examples/simple.ts'
source_filename = "tsc_module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@x = private global double 4.200000e+01
@str = private constant [12 x i8] c"Hello World\00"
@message = private global ptr @str

declare ptr @string_concat(ptr, ptr)

declare void @__throw_exception(ptr)

declare void @__rethrow_exception()

define double @add(double %a, double %b) {
entry:
  %b2 = alloca double, align 8
  %a1 = alloca double, align 8
  store double %a, ptr %a1, align 8
  store double %b, ptr %b2, align 8
  %a_val = load double, ptr %a1, align 8
  %b_val = load double, ptr %b2, align 8
  %add = fadd double %a_val, %b_val
  ret double %add
}

define i32 @main() {
entry:
  ret i32 0
}
