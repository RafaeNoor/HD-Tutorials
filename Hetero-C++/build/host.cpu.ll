; ModuleID = 'src/host.cpp'
source_filename = "src/host.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_Z4copyIiET_PS0_m = comdat any

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %A_vec = alloca i32*, align 8
  %C_vec = alloca i32*, align 8
  %G_vec = alloca i32*, align 8
  %T_vec = alloca i32*, align 8
  %A_encoding = alloca [1024 x i32], align 4
  %C_encoding = alloca [1024 x i32], align 4
  %G_encoding = alloca [1024 x i32], align 4
  %T_encoding = alloca [1024 x i32], align 4
  store i32 0, i32* %retval, align 4
  call void @__hpvm__init() #3
  store i32* null, i32** %A_vec, align 8
  store i32* null, i32** %C_vec, align 8
  store i32* null, i32** %G_vec, align 8
  store i32* null, i32** %T_vec, align 8
  %0 = load i32*, i32** %A_vec, align 8
  %call = call <1024 x i32> (i32, i8*, ...) @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* %0) #3
  %1 = bitcast [1024 x i32]* %A_encoding to <1024 x i32>*
  store <1024 x i32> %call, <1024 x i32>* %1, align 4
  %2 = load i32*, i32** %C_vec, align 8
  %call1 = call <1024 x i32> (i32, i8*, ...) @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* %2) #3
  %3 = bitcast [1024 x i32]* %C_encoding to <1024 x i32>*
  store <1024 x i32> %call1, <1024 x i32>* %3, align 4
  %4 = load i32*, i32** %G_vec, align 8
  %call2 = call <1024 x i32> (i32, i8*, ...) @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* %4) #3
  %5 = bitcast [1024 x i32]* %G_encoding to <1024 x i32>*
  store <1024 x i32> %call2, <1024 x i32>* %5, align 4
  %6 = load i32*, i32** %T_vec, align 8
  %call3 = call <1024 x i32> (i32, i8*, ...) @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* %6) #3
  %7 = bitcast [1024 x i32]* %T_encoding to <1024 x i32>*
  store <1024 x i32> %call3, <1024 x i32>* %7, align 4
  call void @__hpvm__cleanup() #3
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local void @__hpvm__init() #1

; Function Attrs: nounwind
declare dso_local <1024 x i32> @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32, i8*, ...) #1

; Function Attrs: mustprogress noinline nounwind uwtable
define linkonce_odr dso_local i32 @_Z4copyIiET_PS0_m(i32* %data, i64 %loop_index_var) #2 comdat {
entry:
  %data.addr = alloca i32*, align 8
  %loop_index_var.addr = alloca i64, align 8
  store i32* %data, i32** %data.addr, align 8
  store i64 %loop_index_var, i64* %loop_index_var.addr, align 8
  %0 = load i32*, i32** %data.addr, align 8
  %1 = load i64, i64* %loop_index_var.addr, align 8
  %arrayidx = getelementptr inbounds i32, i32* %0, i64 %1
  %2 = load i32, i32* %arrayidx, align 4
  ret i32 %2
}

; Function Attrs: nounwind
declare dso_local void @__hpvm__cleanup() #1

attributes #0 = { mustprogress noinline norecurse nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="32768" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (https://gitlab.engr.illinois.edu/llvm/hpvm.git 0d3b89d33c1e5bac7d2dfde411a32cc53d3e8890)"}
