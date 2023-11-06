; ModuleID = 'cell_field.c'
source_filename = "cell_field.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.CellField = type { [400 x [400 x i32]] }

@CellField_dead_cell = dso_local local_unnamed_addr global i32 0, align 4
@CellField_live_cell = dso_local local_unnamed_addr global i32 1, align 4

; Function Attrs: nounwind uwtable
define dso_local void @CellField_Initialize(%struct.CellField* nocapture noundef writeonly %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1, %5
  %3 = phi i64 [ 0, %1 ], [ %6, %5 ]
  br label %8

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %8
  %6 = add nuw nsw i64 %3, 1
  %7 = icmp eq i64 %6, 400
  br i1 %7, label %4, label %2, !llvm.loop !5

8:                                                ; preds = %2, %8
  %9 = phi i64 [ 0, %2 ], [ %15, %8 ]
  %10 = tail call i32 (...) @GetRandomInteger() #8
  %11 = srem i32 %10, 5
  %12 = icmp eq i32 %11, 0
  %13 = zext i1 %12 to i32
  %14 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %3, i64 %9
  store i32 %13, i32* %14, align 4, !tbaa !7
  %15 = add nuw nsw i64 %9, 1
  %16 = icmp eq i64 %15, 400
  br i1 %16, label %5, label %8, !llvm.loop !11
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

declare i32 @GetRandomInteger(...) local_unnamed_addr #2

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nosync nounwind uwtable
define dso_local void @CellField_Update(%struct.CellField* nocapture noundef %0) local_unnamed_addr #3 {
  %2 = bitcast %struct.CellField* %0 to i8*
  %3 = alloca %struct.CellField, align 4
  %4 = bitcast %struct.CellField* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 640000, i8* nonnull %4) #8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 4 dereferenceable(640000) %4, i8* noundef nonnull align 4 dereferenceable(640000) %2, i64 640000, i1 false), !tbaa !7
  br label %5

5:                                                ; preds = %1, %7
  %6 = phi i32 [ %8, %7 ], [ 0, %1 ]
  br label %11

7:                                                ; preds = %11
  %8 = add nuw nsw i32 %6, 1
  %9 = icmp eq i32 %8, 400
  br i1 %9, label %10, label %5, !llvm.loop !12

10:                                               ; preds = %7
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 4 dereferenceable(640000) %2, i8* noundef nonnull align 4 dereferenceable(640000) %4, i64 640000, i1 false), !tbaa !7
  call void @llvm.lifetime.end.p0i8(i64 640000, i8* nonnull %4) #8
  ret void

11:                                               ; preds = %5, %11
  %12 = phi i32 [ 0, %5 ], [ %13, %11 ]
  call void @CellField_MarkCell(%struct.CellField* noundef %0, %struct.CellField* noundef nonnull %3, i32 noundef %6, i32 noundef %12)
  %13 = add nuw nsw i32 %12, 1
  %14 = icmp eq i32 %13, 400
  br i1 %14, label %7, label %11, !llvm.loop !13
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @CellField_Copy(%struct.CellField* nocapture noundef readonly %0, %struct.CellField* nocapture noundef writeonly %1) local_unnamed_addr #4 {
  br label %3

3:                                                ; preds = %2, %39
  %4 = phi i64 [ 0, %2 ], [ %40, %39 ]
  %5 = add nuw i64 %4, 1
  %6 = getelementptr %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %5, i64 0
  %7 = getelementptr %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 0
  %8 = getelementptr %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %5, i64 0
  %9 = getelementptr %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 0
  %10 = icmp ult i32* %9, %6
  %11 = icmp ult i32* %7, %8
  %12 = and i1 %10, %11
  br i1 %12, label %42, label %13

13:                                               ; preds = %3, %13
  %14 = phi i64 [ %36, %13 ], [ 0, %3 ]
  %15 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %14
  %16 = bitcast i32* %15 to <4 x i32>*
  %17 = load <4 x i32>, <4 x i32>* %16, align 4, !tbaa !7, !alias.scope !14
  %18 = getelementptr inbounds i32, i32* %15, i64 4
  %19 = bitcast i32* %18 to <4 x i32>*
  %20 = load <4 x i32>, <4 x i32>* %19, align 4, !tbaa !7, !alias.scope !14
  %21 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %14
  %22 = bitcast i32* %21 to <4 x i32>*
  store <4 x i32> %17, <4 x i32>* %22, align 4, !tbaa !7, !alias.scope !17, !noalias !14
  %23 = getelementptr inbounds i32, i32* %21, i64 4
  %24 = bitcast i32* %23 to <4 x i32>*
  store <4 x i32> %20, <4 x i32>* %24, align 4, !tbaa !7, !alias.scope !17, !noalias !14
  %25 = or i64 %14, 8
  %26 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %25
  %27 = bitcast i32* %26 to <4 x i32>*
  %28 = load <4 x i32>, <4 x i32>* %27, align 4, !tbaa !7, !alias.scope !14
  %29 = getelementptr inbounds i32, i32* %26, i64 4
  %30 = bitcast i32* %29 to <4 x i32>*
  %31 = load <4 x i32>, <4 x i32>* %30, align 4, !tbaa !7, !alias.scope !14
  %32 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %25
  %33 = bitcast i32* %32 to <4 x i32>*
  store <4 x i32> %28, <4 x i32>* %33, align 4, !tbaa !7, !alias.scope !17, !noalias !14
  %34 = getelementptr inbounds i32, i32* %32, i64 4
  %35 = bitcast i32* %34 to <4 x i32>*
  store <4 x i32> %31, <4 x i32>* %35, align 4, !tbaa !7, !alias.scope !17, !noalias !14
  %36 = add nuw nsw i64 %14, 16
  %37 = icmp eq i64 %36, 400
  br i1 %37, label %39, label %13, !llvm.loop !19

38:                                               ; preds = %39
  ret void

39:                                               ; preds = %13, %42
  %40 = add nuw nsw i64 %4, 1
  %41 = icmp eq i64 %40, 400
  br i1 %41, label %38, label %3, !llvm.loop !21

42:                                               ; preds = %3, %42
  %43 = phi i64 [ %59, %42 ], [ 0, %3 ]
  %44 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %43
  %45 = load i32, i32* %44, align 4, !tbaa !7
  %46 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %43
  store i32 %45, i32* %46, align 4, !tbaa !7
  %47 = or i64 %43, 1
  %48 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %47
  %49 = load i32, i32* %48, align 4, !tbaa !7
  %50 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %47
  store i32 %49, i32* %50, align 4, !tbaa !7
  %51 = or i64 %43, 2
  %52 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %51
  %53 = load i32, i32* %52, align 4, !tbaa !7
  %54 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %51
  store i32 %53, i32* %54, align 4, !tbaa !7
  %55 = or i64 %43, 3
  %56 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %4, i64 %55
  %57 = load i32, i32* %56, align 4, !tbaa !7
  %58 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %4, i64 %55
  store i32 %57, i32* %58, align 4, !tbaa !7
  %59 = add nuw nsw i64 %43, 4
  %60 = icmp eq i64 %59, 400
  br i1 %60, label %39, label %42, !llvm.loop !22
}

; Function Attrs: nofree nosync nounwind uwtable
define dso_local void @CellField_MarkCell(%struct.CellField* nocapture noundef readonly %0, %struct.CellField* nocapture noundef writeonly %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #3 {
  %5 = icmp eq i32 %2, 0
  %6 = add nsw i32 %2, -1
  %7 = select i1 %5, i32 399, i32 %6
  %8 = icmp eq i32 %2, 399
  %9 = add nsw i32 %2, 1
  %10 = select i1 %8, i32 0, i32 %9
  %11 = icmp eq i32 %3, 0
  %12 = add nsw i32 %3, -1
  %13 = select i1 %11, i32 399, i32 %12
  %14 = icmp eq i32 %3, 399
  %15 = add nsw i32 %3, 1
  %16 = select i1 %14, i32 0, i32 %15
  %17 = sext i32 %7 to i64
  %18 = sext i32 %13 to i64
  %19 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %18
  %20 = load i32, i32* %19, align 4, !tbaa !7
  %21 = sext i32 %3 to i64
  %22 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %21
  %23 = load i32, i32* %22, align 4, !tbaa !7
  %24 = sext i32 %16 to i64
  %25 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %24
  %26 = load i32, i32* %25, align 4, !tbaa !7
  %27 = sext i32 %2 to i64
  %28 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %27, i64 %18
  %29 = load i32, i32* %28, align 4, !tbaa !7
  %30 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %27, i64 %24
  %31 = load i32, i32* %30, align 4, !tbaa !7
  %32 = sext i32 %10 to i64
  %33 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %32, i64 %18
  %34 = load i32, i32* %33, align 4, !tbaa !7
  %35 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %32, i64 %21
  %36 = load i32, i32* %35, align 4, !tbaa !7
  %37 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %32, i64 %24
  %38 = load i32, i32* %37, align 4, !tbaa !7
  %39 = load i32, i32* @CellField_dead_cell, align 4, !tbaa !7
  %40 = icmp ne i32 %39, %20
  %41 = zext i1 %40 to i32
  %42 = icmp ne i32 %39, %23
  %43 = zext i1 %42 to i32
  %44 = add nuw nsw i32 %41, %43
  %45 = icmp ne i32 %39, %26
  %46 = zext i1 %45 to i32
  %47 = add nuw nsw i32 %44, %46
  %48 = icmp ne i32 %39, %29
  %49 = zext i1 %48 to i32
  %50 = add nuw nsw i32 %47, %49
  %51 = icmp ne i32 %39, %31
  %52 = zext i1 %51 to i32
  %53 = add nuw nsw i32 %50, %52
  %54 = icmp ne i32 %39, %34
  %55 = zext i1 %54 to i32
  %56 = add nuw nsw i32 %53, %55
  %57 = icmp ne i32 %39, %36
  %58 = zext i1 %57 to i32
  %59 = add nuw nsw i32 %56, %58
  %60 = icmp ne i32 %39, %38
  %61 = zext i1 %60 to i32
  %62 = add nuw nsw i32 %59, %61
  %63 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %27, i64 %21
  %64 = load i32, i32* %63, align 4, !tbaa !7
  %65 = icmp eq i32 %39, %64
  br i1 %65, label %66, label %70

66:                                               ; preds = %4
  %67 = icmp eq i32 %62, 3
  br i1 %67, label %68, label %76

68:                                               ; preds = %66
  %69 = load i32, i32* @CellField_live_cell, align 4, !tbaa !7
  br label %73

70:                                               ; preds = %4
  %71 = and i32 %62, 2147483646
  %72 = icmp eq i32 %71, 2
  br i1 %72, label %76, label %73

73:                                               ; preds = %70, %68
  %74 = phi i32 [ %69, %68 ], [ %39, %70 ]
  %75 = getelementptr inbounds %struct.CellField, %struct.CellField* %1, i64 0, i32 0, i64 %27, i64 %21
  store i32 %74, i32* %75, align 4, !tbaa !7
  br label %76

76:                                               ; preds = %73, %70, %66
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind uwtable willreturn
define dso_local void @CellField_SetNearbyCells(%struct.CellField* nocapture noundef readonly %0, i32* nocapture noundef writeonly %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #5 {
  %5 = icmp eq i32 %2, 0
  %6 = add nsw i32 %2, -1
  %7 = select i1 %5, i32 399, i32 %6
  %8 = icmp eq i32 %2, 399
  %9 = add nsw i32 %2, 1
  %10 = select i1 %8, i32 0, i32 %9
  %11 = icmp eq i32 %3, 0
  %12 = add nsw i32 %3, -1
  %13 = select i1 %11, i32 399, i32 %12
  %14 = icmp eq i32 %3, 399
  %15 = add nsw i32 %3, 1
  %16 = select i1 %14, i32 0, i32 %15
  %17 = sext i32 %7 to i64
  %18 = sext i32 %13 to i64
  %19 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %18
  %20 = load i32, i32* %19, align 4, !tbaa !7
  store i32 %20, i32* %1, align 4, !tbaa !7
  %21 = sext i32 %3 to i64
  %22 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %21
  %23 = load i32, i32* %22, align 4, !tbaa !7
  %24 = getelementptr inbounds i32, i32* %1, i64 1
  store i32 %23, i32* %24, align 4, !tbaa !7
  %25 = sext i32 %16 to i64
  %26 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %17, i64 %25
  %27 = load i32, i32* %26, align 4, !tbaa !7
  %28 = getelementptr inbounds i32, i32* %1, i64 2
  store i32 %27, i32* %28, align 4, !tbaa !7
  %29 = sext i32 %2 to i64
  %30 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %29, i64 %18
  %31 = load i32, i32* %30, align 4, !tbaa !7
  %32 = getelementptr inbounds i32, i32* %1, i64 3
  store i32 %31, i32* %32, align 4, !tbaa !7
  %33 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %29, i64 %25
  %34 = load i32, i32* %33, align 4, !tbaa !7
  %35 = getelementptr inbounds i32, i32* %1, i64 4
  store i32 %34, i32* %35, align 4, !tbaa !7
  %36 = sext i32 %10 to i64
  %37 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %36, i64 %18
  %38 = load i32, i32* %37, align 4, !tbaa !7
  %39 = getelementptr inbounds i32, i32* %1, i64 5
  store i32 %38, i32* %39, align 4, !tbaa !7
  %40 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %36, i64 %21
  %41 = load i32, i32* %40, align 4, !tbaa !7
  %42 = getelementptr inbounds i32, i32* %1, i64 6
  store i32 %41, i32* %42, align 4, !tbaa !7
  %43 = getelementptr inbounds %struct.CellField, %struct.CellField* %0, i64 0, i32 0, i64 %36, i64 %25
  %44 = load i32, i32* %43, align 4, !tbaa !7
  %45 = getelementptr inbounds i32, i32* %1, i64 7
  store i32 %44, i32* %45, align 4, !tbaa !7
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readonly uwtable willreturn
define dso_local i32 @CellField_IsCellDead(i32 noundef %0) local_unnamed_addr #6 {
  %2 = load i32, i32* @CellField_dead_cell, align 4, !tbaa !7
  %3 = icmp eq i32 %2, %0
  %4 = zext i1 %3 to i32
  ret i32 %4
}




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

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #7

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree norecurse nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree norecurse nosync nounwind uwtable willreturn "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nofree norecurse nosync nounwind readonly uwtable willreturn "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { argmemonly nofree nounwind willreturn }
attributes #8 = { nounwind }

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
!13 = distinct !{!13, !6}
!14 = !{!15}
!15 = distinct !{!15, !16}
!16 = distinct !{!16, !"LVerDomain"}
!17 = !{!18}
!18 = distinct !{!18, !16}
!19 = distinct !{!19, !6, !20}
!20 = !{!"llvm.loop.isvectorized", i32 1}
!21 = distinct !{!21, !6}
!22 = distinct !{!22, !6, !20}
