; ModuleID = 'build/hpvmc.cpu.ll'
source_filename = "src/host.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_Z4copyIiET_PS0_m = comdat any

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
entry:
  %call374 = call i8* @malloc(i64 4096)
  %call263 = call i8* @malloc(i64 4096)
  %call152 = call i8* @malloc(i64 4096)
  %call41 = call i8* @malloc(i64 4096)
  %0 = bitcast i8* %call41 to <1024 x i32>*
  tail call void @__hpvm__init() #3
  br label %loop.header

loop.header:                                      ; preds = %loop.header, %entry
  %loop.iv = phi i64 [ 0, %entry ], [ %loop.step, %loop.header ]
  %arrayidx.i = getelementptr inbounds i32, i32* null, i64 %loop.iv
  %1 = load i32, i32* %arrayidx.i, align 4
  %2 = bitcast i32 %1 to i32
  %3 = bitcast <1024 x i32>* %0 to i32*
  %4 = getelementptr i32, i32* %3, i64 %loop.iv
  store i32 %2, i32* %4, align 4
  %loop.step = add nuw nsw i64 %loop.iv, 1
  %exitcond3 = icmp ne i64 %loop.step, 1024
  br i1 %exitcond3, label %loop.header, label %loop.header7.preheader, !llvm.loop !4

loop.header7.preheader:                           ; preds = %loop.header
  %5 = bitcast i8* %call374 to <1024 x i32>*
  %6 = bitcast i8* %call263 to <1024 x i32>*
  %7 = bitcast i8* %call152 to <1024 x i32>*
  br label %loop.header7

loop.header7:                                     ; preds = %loop.header7, %loop.header7.preheader
  %loop.iv10 = phi i64 [ %loop.step11, %loop.header7 ], [ 0, %loop.header7.preheader ]
  %arrayidx.i13 = getelementptr inbounds i32, i32* null, i64 %loop.iv10
  %8 = load i32, i32* %arrayidx.i13, align 4
  %9 = bitcast i32 %8 to i32
  %10 = bitcast <1024 x i32>* %7 to i32*
  %11 = getelementptr i32, i32* %10, i64 %loop.iv10
  store i32 %9, i32* %11, align 4
  %loop.step11 = add nuw nsw i64 %loop.iv10, 1
  %exitcond2 = icmp ne i64 %loop.step11, 1024
  br i1 %exitcond2, label %loop.header7, label %loop.header16, !llvm.loop !6

loop.header16:                                    ; preds = %loop.header16, %loop.header7
  %loop.iv19 = phi i64 [ %loop.step20, %loop.header16 ], [ 0, %loop.header7 ]
  %arrayidx.i22 = getelementptr inbounds i32, i32* null, i64 %loop.iv19
  %12 = load i32, i32* %arrayidx.i22, align 4
  %13 = bitcast i32 %12 to i32
  %14 = bitcast <1024 x i32>* %6 to i32*
  %15 = getelementptr i32, i32* %14, i64 %loop.iv19
  store i32 %13, i32* %15, align 4
  %loop.step20 = add nuw nsw i64 %loop.iv19, 1
  %exitcond1 = icmp ne i64 %loop.step20, 1024
  br i1 %exitcond1, label %loop.header16, label %loop.header25, !llvm.loop !7

loop.header25:                                    ; preds = %loop.header25, %loop.header16
  %loop.iv28 = phi i64 [ %loop.step29, %loop.header25 ], [ 0, %loop.header16 ]
  %arrayidx.i31 = getelementptr inbounds i32, i32* null, i64 %loop.iv28
  %16 = load i32, i32* %arrayidx.i31, align 4
  %17 = bitcast i32 %16 to i32
  %18 = bitcast <1024 x i32>* %5 to i32*
  %19 = getelementptr i32, i32* %18, i64 %loop.iv28
  store i32 %17, i32* %19, align 4
  %loop.step29 = add nuw nsw i64 %loop.iv28, 1
  %exitcond = icmp ne i64 %loop.step29, 1024
  br i1 %exitcond, label %loop.header25, label %continue24, !llvm.loop !8

continue24:                                       ; preds = %loop.header25
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

declare i8* @malloc(i64)

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(i8* nocapture noundef readonly, ...) #4

; Function Attrs: nounwind
declare dso_local void @__hpvm__hint(i32) #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__createNodeND(i32, ...) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__return(i32, ...) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__attributes(i32, ...) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__bindIn(i8*, i32, i32, i32) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__bindOut(i8*, i32, i32, i32) #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__edge(i8*, i8*, i32, i32, i32, i32) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__push(i8*, i8*) #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__pop(i8*) #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__launch(i32, ...) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__wait(i8*) #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__getNode() #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__getParentNode(i8*) #5

; Function Attrs: nounwind
declare dso_local void @__hpvm__barrier() #5

; Function Attrs: nounwind
declare dso_local i8* @__hpvm__malloc(i64) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNodeInstanceID_x(i8*) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNodeInstanceID_y(i8*) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNodeInstanceID_z(i8*) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNumNodeInstances_x(i8*) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNumNodeInstances_y(i8*) #5

; Function Attrs: nounwind
declare dso_local i64 @__hpvm__getNumNodeInstances_z(i8*) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_add(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_sub(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_xchg(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_inc(i32*) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_dec(i32*) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_min(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_max(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_and(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_or(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local i32 @__hpvm__atomic_xor(i32*, i32) #5

; Function Attrs: nounwind
declare dso_local void @llvm_hpvm_track_mem(i8*, i64) #5

; Function Attrs: nounwind
declare dso_local void @llvm_hpvm_untrack_mem(i8*) #5

; Function Attrs: nounwind
declare dso_local void @llvm_hpvm_request_mem(i8*, i64) #5

attributes #0 = { mustprogress noinline norecurse nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="32768" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }
attributes #4 = { nofree nounwind "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 13.0.0 (https://gitlab.engr.illinois.edu/llvm/hpvm.git 0d3b89d33c1e5bac7d2dfde411a32cc53d3e8890)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.unroll.disable"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
