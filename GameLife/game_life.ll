; ModuleID = 'game_life.c'
source_filename = "game_life.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.CellField = type { [400 x [400 x i32]] }

; Function Attrs: nounwind uwtable
define dso_local void @SetPixels(%struct.CellField* nocapture noundef readonly %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1, %6
  %3 = phi i64 [ 0, %1 ], [ %7, %6 ]
  %4 = trunc i64 %3 to i32
  br label %9

5:                                                ; preds = %6
  ret void

6:                                                ; preds = %9
  %7 = add nuw nsw i64 %3, 1
  %8 = icmp eq i64 %7, 400
  br i1 %8, label %5, label %2, !llvm.loop !5

9:                                                ; preds = %2, %9
  %10 = phi i64 [ 0, %2 ], [ %14, %9 ]
  %11 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %3, i64 %10
  %12 = load i32, i32* %11, align 4, !tbaa !7
  %13 = trunc i64 %10 to i32
  tail call void @SetPixel(i32 noundef %13, i32 noundef %4, i32 noundef %12) #3
  %14 = add nuw nsw i64 %10, 1
  %15 = icmp eq i64 %14, 400
  br i1 %15, label %6, label %9, !llvm.loop !11
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

declare void @SetPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %struct.CellField, align 4
  %2 = tail call i32 @CreateGraphicResources(i32 noundef 400, i32 noundef 400) #3
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %26, label %4

4:                                                ; preds = %0
  %5 = bitcast %struct.CellField* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 640000, i8* nonnull %5) #3
  call void @CellField_Initialize(%struct.CellField* noundef nonnull %1) #3
  %6 = call i32 (...) @RenderWindowIsOpen() #3
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %25, label %8

8:                                                ; preds = %4, %22
  call void (...) @ClearFrame() #3
  call void @CellField_Update(%struct.CellField* noundef nonnull %1) #3
  br label %9

9:                                                ; preds = %12, %8
  %10 = phi i64 [ 0, %8 ], [ %13, %12 ]
  %11 = trunc i64 %10 to i32
  br label %15

12:                                               ; preds = %15
  %13 = add nuw nsw i64 %10, 1
  %14 = icmp eq i64 %13, 400
  br i1 %14, label %22, label %9, !llvm.loop !5

15:                                               ; preds = %15, %9
  %16 = phi i64 [ 0, %9 ], [ %20, %15 ]
  %17 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %10, i64 %16
  %18 = load i32, i32* %17, align 4, !tbaa !7
  %19 = trunc i64 %16 to i32
  call void @SetPixel(i32 noundef %19, i32 noundef %11, i32 noundef %18) #3
  %20 = add nuw nsw i64 %16, 1
  %21 = icmp eq i64 %20, 400
  br i1 %21, label %12, label %15, !llvm.loop !11

22:                                               ; preds = %12
  call void (...) @DisplayFrame() #3
  %23 = call i32 (...) @RenderWindowIsOpen() #3
  %24 = icmp eq i32 %23, 0
  br i1 %24, label %25, label %8, !llvm.loop !12

25:                                               ; preds = %22, %4
  call void (...) @DestroyGraphicResources() #3
  call void @llvm.lifetime.end.p0i8(i64 640000, i8* nonnull %5) #3
  br label %26

26:                                               ; preds = %0, %25
  %27 = phi i32 [ 0, %25 ], [ 1, %0 ]
  ret i32 %27
}

declare i32 @CreateGraphicResources(i32 noundef, i32 noundef) local_unnamed_addr #2

declare void @CellField_Initialize(%struct.CellField* noundef) local_unnamed_addr #2

declare i32 @RenderWindowIsOpen(...) local_unnamed_addr #2

declare void @ClearFrame(...) local_unnamed_addr #2

declare void @CellField_Update(%struct.CellField* noundef) local_unnamed_addr #2

declare void @DisplayFrame(...) local_unnamed_addr #2

declare void @DestroyGraphicResources(...) local_unnamed_addr #2

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
!11 = distinct !{!11, !6}
!12 = distinct !{!12, !6}
