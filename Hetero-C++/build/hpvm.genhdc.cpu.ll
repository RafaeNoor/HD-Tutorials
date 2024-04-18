; ModuleID = 'build/host.cpu.ll'
source_filename = "src/host.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_Z4copyIiET_PS0_m = comdat any

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
entry:
  tail call void @__hpvm__init() #3
  %call4 = call <1024 x i32> (i32, i8*, ...) @llvm.hpvm.hdc.create.hypervector.v1024i32(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* null)
  %call15 = call <1024 x i32> (i32, i8*, ...) @llvm.hpvm.hdc.create.hypervector.v1024i32(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* null)
  %call26 = call <1024 x i32> (i32, i8*, ...) @llvm.hpvm.hdc.create.hypervector.v1024i32(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* null)
  %call37 = call <1024 x i32> (i32, i8*, ...) @llvm.hpvm.hdc.create.hypervector.v1024i32(i32 1, i8* bitcast (i32 (i32*, i64)* @_Z4copyIiET_PS0_m to i8*), i32* null)
  tail call void @__hpvm__cleanup() #3
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local void @__hpvm__init() local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local <1024 x i32> @_Z31__hetero_hdc_create_hypervectorILi1024EiEu11matrix_typeILi1EXT_ET0_EiPvz(i32, i8*, ...) local_unnamed_addr #1

; Function Attrs: mustprogress noinline nounwind uwtable
define linkonce_odr dso_local i32 @_Z4copyIiET_PS0_m(i32* %data, i64 %loop_index_var) #2 comdat {
entry:
  %arrayidx = getelementptr inbounds i32, i32* %data, i64 %loop_index_var
  %0 = load i32, i32* %arrayidx, align 4
  ret i32 %0
}

; Function Attrs: nounwind
declare dso_local void @__hpvm__cleanup() local_unnamed_addr #1

; Function Attrs: nounwind
declare <1024 x i32> @llvm.hpvm.hdc.create.hypervector.v1024i32(i32, i8*, ...) #3

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
