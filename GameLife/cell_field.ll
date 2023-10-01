; ModuleID = 'cell_field.c'
source_filename = "cell_field.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.CellField = type { [720 x [1280 x i32]] }

@CellField_dead_cell = dso_local global i32 0, align 4
@CellField_live_cell = dso_local global i32 1, align 4
@CellField_MarkCell.nearby_cells_cnt = internal constant i32 8, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CellField_Initialize(%struct.CellField* noundef %0) #0 {
  %2 = alloca %struct.CellField*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store %struct.CellField* %0, %struct.CellField** %2, align 8
  store i32 0, i32* %3, align 4
  br label %5

5:                                                ; preds = %30, %1
  %6 = load i32, i32* %3, align 4
  %7 = icmp slt i32 %6, 720
  br i1 %7, label %8, label %33

8:                                                ; preds = %5
  store i32 0, i32* %4, align 4
  br label %9

9:                                                ; preds = %26, %8
  %10 = load i32, i32* %4, align 4
  %11 = icmp slt i32 %10, 1280
  br i1 %11, label %12, label %29

12:                                               ; preds = %9
  %13 = call i32 (...) @GetRandomInteger()
  %14 = srem i32 %13, 5
  %15 = icmp ne i32 %14, 0
  %16 = xor i1 %15, true
  %17 = zext i1 %16 to i32
  %18 = load %struct.CellField*, %struct.CellField** %2, align 8
  %19 = getelementptr inbounds %struct.CellField, %struct.CellField* %18, i32 0, i32 0
  %20 = load i32, i32* %3, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %19, i64 0, i64 %21
  %23 = load i32, i32* %4, align 4
  %24 = sext i32 %23 to i64
  %25 = getelementptr inbounds [1280 x i32], [1280 x i32]* %22, i64 0, i64 %24
  store i32 %17, i32* %25, align 4
  br label %26

26:                                               ; preds = %12
  %27 = load i32, i32* %4, align 4
  %28 = add nsw i32 %27, 1
  store i32 %28, i32* %4, align 4
  br label %9, !llvm.loop !6

29:                                               ; preds = %9
  br label %30

30:                                               ; preds = %29
  %31 = load i32, i32* %3, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, i32* %3, align 4
  br label %5, !llvm.loop !8

33:                                               ; preds = %5
  ret void
}

declare i32 @GetRandomInteger(...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CellField_Update(%struct.CellField* noundef %0) #0 {
  %2 = alloca %struct.CellField*, align 8
  %3 = alloca %struct.CellField, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store %struct.CellField* %0, %struct.CellField** %2, align 8
  %6 = load %struct.CellField*, %struct.CellField** %2, align 8
  call void @CellField_Copy(%struct.CellField* noundef %6, %struct.CellField* noundef %3)
  store i32 0, i32* %4, align 4
  br label %7

7:                                                ; preds = %22, %1
  %8 = load i32, i32* %4, align 4
  %9 = icmp slt i32 %8, 720
  br i1 %9, label %10, label %25

10:                                               ; preds = %7
  store i32 0, i32* %5, align 4
  br label %11

11:                                               ; preds = %18, %10
  %12 = load i32, i32* %5, align 4
  %13 = icmp slt i32 %12, 1280
  br i1 %13, label %14, label %21

14:                                               ; preds = %11
  %15 = load %struct.CellField*, %struct.CellField** %2, align 8
  %16 = load i32, i32* %4, align 4
  %17 = load i32, i32* %5, align 4
  call void @CellField_MarkCell(%struct.CellField* noundef %15, %struct.CellField* noundef %3, i32 noundef %16, i32 noundef %17)
  br label %18

18:                                               ; preds = %14
  %19 = load i32, i32* %5, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, i32* %5, align 4
  br label %11, !llvm.loop !9

21:                                               ; preds = %11
  br label %22

22:                                               ; preds = %21
  %23 = load i32, i32* %4, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, i32* %4, align 4
  br label %7, !llvm.loop !10

25:                                               ; preds = %7
  %26 = load %struct.CellField*, %struct.CellField** %2, align 8
  call void @CellField_Copy(%struct.CellField* noundef %3, %struct.CellField* noundef %26)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CellField_Copy(%struct.CellField* noundef %0, %struct.CellField* noundef %1) #0 {
  %3 = alloca %struct.CellField*, align 8
  %4 = alloca %struct.CellField*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store %struct.CellField* %0, %struct.CellField** %3, align 8
  store %struct.CellField* %1, %struct.CellField** %4, align 8
  store i32 0, i32* %5, align 4
  br label %7

7:                                                ; preds = %36, %2
  %8 = load i32, i32* %5, align 4
  %9 = icmp slt i32 %8, 720
  br i1 %9, label %10, label %39

10:                                               ; preds = %7
  store i32 0, i32* %6, align 4
  br label %11

11:                                               ; preds = %32, %10
  %12 = load i32, i32* %6, align 4
  %13 = icmp slt i32 %12, 1280
  br i1 %13, label %14, label %35

14:                                               ; preds = %11
  %15 = load %struct.CellField*, %struct.CellField** %3, align 8
  %16 = getelementptr inbounds %struct.CellField, %struct.CellField* %15, i32 0, i32 0
  %17 = load i32, i32* %5, align 4
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %16, i64 0, i64 %18
  %20 = load i32, i32* %6, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds [1280 x i32], [1280 x i32]* %19, i64 0, i64 %21
  %23 = load i32, i32* %22, align 4
  %24 = load %struct.CellField*, %struct.CellField** %4, align 8
  %25 = getelementptr inbounds %struct.CellField, %struct.CellField* %24, i32 0, i32 0
  %26 = load i32, i32* %5, align 4
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %25, i64 0, i64 %27
  %29 = load i32, i32* %6, align 4
  %30 = sext i32 %29 to i64
  %31 = getelementptr inbounds [1280 x i32], [1280 x i32]* %28, i64 0, i64 %30
  store i32 %23, i32* %31, align 4
  br label %32

32:                                               ; preds = %14
  %33 = load i32, i32* %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, i32* %6, align 4
  br label %11, !llvm.loop !11

35:                                               ; preds = %11
  br label %36

36:                                               ; preds = %35
  %37 = load i32, i32* %5, align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, i32* %5, align 4
  br label %7, !llvm.loop !12

39:                                               ; preds = %7
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CellField_MarkCell(%struct.CellField* noundef %0, %struct.CellField* noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca %struct.CellField*, align 8
  %6 = alloca %struct.CellField*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca [8 x i32], align 16
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store %struct.CellField* %0, %struct.CellField** %5, align 8
  store %struct.CellField* %1, %struct.CellField** %6, align 8
  store i32 %2, i32* %7, align 4
  store i32 %3, i32* %8, align 4
  %12 = bitcast [8 x i32]* %9 to i8*
  call void @llvm.memset.p0i8.i64(i8* align 16 %12, i8 0, i64 32, i1 false)
  %13 = load %struct.CellField*, %struct.CellField** %5, align 8
  %14 = getelementptr inbounds [8 x i32], [8 x i32]* %9, i64 0, i64 0
  %15 = load i32, i32* %7, align 4
  %16 = load i32, i32* %8, align 4
  call void @CellField_SetNearbyCells(%struct.CellField* noundef %13, i32* noundef %14, i32 noundef %15, i32 noundef %16)
  store i32 0, i32* %10, align 4
  store i32 0, i32* %11, align 4
  br label %17

17:                                               ; preds = %31, %4
  %18 = load i32, i32* %11, align 4
  %19 = icmp slt i32 %18, 8
  br i1 %19, label %20, label %34

20:                                               ; preds = %17
  %21 = load i32, i32* %11, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds [8 x i32], [8 x i32]* %9, i64 0, i64 %22
  %24 = load i32, i32* %23, align 4
  %25 = call i32 @CellField_IsCellDead(i32 noundef %24)
  %26 = icmp ne i32 %25, 0
  br i1 %26, label %30, label %27

27:                                               ; preds = %20
  %28 = load i32, i32* %10, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* %10, align 4
  br label %30

30:                                               ; preds = %27, %20
  br label %31

31:                                               ; preds = %30
  %32 = load i32, i32* %11, align 4
  %33 = add nsw i32 %32, 1
  store i32 %33, i32* %11, align 4
  br label %17, !llvm.loop !13

34:                                               ; preds = %17
  %35 = load %struct.CellField*, %struct.CellField** %5, align 8
  %36 = getelementptr inbounds %struct.CellField, %struct.CellField* %35, i32 0, i32 0
  %37 = load i32, i32* %7, align 4
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %36, i64 0, i64 %38
  %40 = load i32, i32* %8, align 4
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds [1280 x i32], [1280 x i32]* %39, i64 0, i64 %41
  %43 = load i32, i32* %42, align 4
  %44 = call i32 @CellField_IsCellDead(i32 noundef %43)
  %45 = icmp ne i32 %44, 0
  br i1 %45, label %46, label %60

46:                                               ; preds = %34
  %47 = load i32, i32* %10, align 4
  %48 = icmp eq i32 %47, 3
  br i1 %48, label %49, label %59

49:                                               ; preds = %46
  %50 = load i32, i32* @CellField_live_cell, align 4
  %51 = load %struct.CellField*, %struct.CellField** %6, align 8
  %52 = getelementptr inbounds %struct.CellField, %struct.CellField* %51, i32 0, i32 0
  %53 = load i32, i32* %7, align 4
  %54 = sext i32 %53 to i64
  %55 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %52, i64 0, i64 %54
  %56 = load i32, i32* %8, align 4
  %57 = sext i32 %56 to i64
  %58 = getelementptr inbounds [1280 x i32], [1280 x i32]* %55, i64 0, i64 %57
  store i32 %50, i32* %58, align 4
  br label %59

59:                                               ; preds = %49, %46
  br label %77

60:                                               ; preds = %34
  %61 = load i32, i32* %10, align 4
  %62 = icmp ne i32 %61, 2
  br i1 %62, label %63, label %76

63:                                               ; preds = %60
  %64 = load i32, i32* %10, align 4
  %65 = icmp ne i32 %64, 3
  br i1 %65, label %66, label %76

66:                                               ; preds = %63
  %67 = load i32, i32* @CellField_dead_cell, align 4
  %68 = load %struct.CellField*, %struct.CellField** %6, align 8
  %69 = getelementptr inbounds %struct.CellField, %struct.CellField* %68, i32 0, i32 0
  %70 = load i32, i32* %7, align 4
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %69, i64 0, i64 %71
  %73 = load i32, i32* %8, align 4
  %74 = sext i32 %73 to i64
  %75 = getelementptr inbounds [1280 x i32], [1280 x i32]* %72, i64 0, i64 %74
  store i32 %67, i32* %75, align 4
  br label %76

76:                                               ; preds = %66, %63, %60
  br label %77

77:                                               ; preds = %76, %59
  ret void
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CellField_SetNearbyCells(%struct.CellField* noundef %0, i32* noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca %struct.CellField*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  store %struct.CellField* %0, %struct.CellField** %5, align 8
  store i32* %1, i32** %6, align 8
  store i32 %2, i32* %7, align 4
  store i32 %3, i32* %8, align 4
  %13 = load i32, i32* %7, align 4
  %14 = icmp ne i32 %13, 0
  br i1 %14, label %15, label %18

15:                                               ; preds = %4
  %16 = load i32, i32* %7, align 4
  %17 = sub nsw i32 %16, 1
  br label %19

18:                                               ; preds = %4
  br label %19

19:                                               ; preds = %18, %15
  %20 = phi i32 [ %17, %15 ], [ 719, %18 ]
  store i32 %20, i32* %9, align 4
  %21 = load i32, i32* %7, align 4
  %22 = icmp ne i32 %21, 719
  br i1 %22, label %23, label %26

23:                                               ; preds = %19
  %24 = load i32, i32* %7, align 4
  %25 = add nsw i32 %24, 1
  br label %27

26:                                               ; preds = %19
  br label %27

27:                                               ; preds = %26, %23
  %28 = phi i32 [ %25, %23 ], [ 0, %26 ]
  store i32 %28, i32* %10, align 4
  %29 = load i32, i32* %8, align 4
  %30 = icmp ne i32 %29, 0
  br i1 %30, label %31, label %34

31:                                               ; preds = %27
  %32 = load i32, i32* %8, align 4
  %33 = sub nsw i32 %32, 1
  br label %35

34:                                               ; preds = %27
  br label %35

35:                                               ; preds = %34, %31
  %36 = phi i32 [ %33, %31 ], [ 1279, %34 ]
  store i32 %36, i32* %11, align 4
  %37 = load i32, i32* %8, align 4
  %38 = icmp ne i32 %37, 1279
  br i1 %38, label %39, label %42

39:                                               ; preds = %35
  %40 = load i32, i32* %8, align 4
  %41 = add nsw i32 %40, 1
  br label %43

42:                                               ; preds = %35
  br label %43

43:                                               ; preds = %42, %39
  %44 = phi i32 [ %41, %39 ], [ 0, %42 ]
  store i32 %44, i32* %12, align 4
  %45 = load %struct.CellField*, %struct.CellField** %5, align 8
  %46 = getelementptr inbounds %struct.CellField, %struct.CellField* %45, i32 0, i32 0
  %47 = load i32, i32* %9, align 4
  %48 = sext i32 %47 to i64
  %49 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %46, i64 0, i64 %48
  %50 = load i32, i32* %11, align 4
  %51 = sext i32 %50 to i64
  %52 = getelementptr inbounds [1280 x i32], [1280 x i32]* %49, i64 0, i64 %51
  %53 = load i32, i32* %52, align 4
  %54 = load i32*, i32** %6, align 8
  %55 = getelementptr inbounds i32, i32* %54, i64 0
  store i32 %53, i32* %55, align 4
  %56 = load %struct.CellField*, %struct.CellField** %5, align 8
  %57 = getelementptr inbounds %struct.CellField, %struct.CellField* %56, i32 0, i32 0
  %58 = load i32, i32* %9, align 4
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %57, i64 0, i64 %59
  %61 = load i32, i32* %8, align 4
  %62 = sext i32 %61 to i64
  %63 = getelementptr inbounds [1280 x i32], [1280 x i32]* %60, i64 0, i64 %62
  %64 = load i32, i32* %63, align 4
  %65 = load i32*, i32** %6, align 8
  %66 = getelementptr inbounds i32, i32* %65, i64 1
  store i32 %64, i32* %66, align 4
  %67 = load %struct.CellField*, %struct.CellField** %5, align 8
  %68 = getelementptr inbounds %struct.CellField, %struct.CellField* %67, i32 0, i32 0
  %69 = load i32, i32* %9, align 4
  %70 = sext i32 %69 to i64
  %71 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %68, i64 0, i64 %70
  %72 = load i32, i32* %12, align 4
  %73 = sext i32 %72 to i64
  %74 = getelementptr inbounds [1280 x i32], [1280 x i32]* %71, i64 0, i64 %73
  %75 = load i32, i32* %74, align 4
  %76 = load i32*, i32** %6, align 8
  %77 = getelementptr inbounds i32, i32* %76, i64 2
  store i32 %75, i32* %77, align 4
  %78 = load %struct.CellField*, %struct.CellField** %5, align 8
  %79 = getelementptr inbounds %struct.CellField, %struct.CellField* %78, i32 0, i32 0
  %80 = load i32, i32* %7, align 4
  %81 = sext i32 %80 to i64
  %82 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %79, i64 0, i64 %81
  %83 = load i32, i32* %11, align 4
  %84 = sext i32 %83 to i64
  %85 = getelementptr inbounds [1280 x i32], [1280 x i32]* %82, i64 0, i64 %84
  %86 = load i32, i32* %85, align 4
  %87 = load i32*, i32** %6, align 8
  %88 = getelementptr inbounds i32, i32* %87, i64 3
  store i32 %86, i32* %88, align 4
  %89 = load %struct.CellField*, %struct.CellField** %5, align 8
  %90 = getelementptr inbounds %struct.CellField, %struct.CellField* %89, i32 0, i32 0
  %91 = load i32, i32* %7, align 4
  %92 = sext i32 %91 to i64
  %93 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %90, i64 0, i64 %92
  %94 = load i32, i32* %12, align 4
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds [1280 x i32], [1280 x i32]* %93, i64 0, i64 %95
  %97 = load i32, i32* %96, align 4
  %98 = load i32*, i32** %6, align 8
  %99 = getelementptr inbounds i32, i32* %98, i64 4
  store i32 %97, i32* %99, align 4
  %100 = load %struct.CellField*, %struct.CellField** %5, align 8
  %101 = getelementptr inbounds %struct.CellField, %struct.CellField* %100, i32 0, i32 0
  %102 = load i32, i32* %10, align 4
  %103 = sext i32 %102 to i64
  %104 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %101, i64 0, i64 %103
  %105 = load i32, i32* %11, align 4
  %106 = sext i32 %105 to i64
  %107 = getelementptr inbounds [1280 x i32], [1280 x i32]* %104, i64 0, i64 %106
  %108 = load i32, i32* %107, align 4
  %109 = load i32*, i32** %6, align 8
  %110 = getelementptr inbounds i32, i32* %109, i64 5
  store i32 %108, i32* %110, align 4
  %111 = load %struct.CellField*, %struct.CellField** %5, align 8
  %112 = getelementptr inbounds %struct.CellField, %struct.CellField* %111, i32 0, i32 0
  %113 = load i32, i32* %10, align 4
  %114 = sext i32 %113 to i64
  %115 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %112, i64 0, i64 %114
  %116 = load i32, i32* %8, align 4
  %117 = sext i32 %116 to i64
  %118 = getelementptr inbounds [1280 x i32], [1280 x i32]* %115, i64 0, i64 %117
  %119 = load i32, i32* %118, align 4
  %120 = load i32*, i32** %6, align 8
  %121 = getelementptr inbounds i32, i32* %120, i64 6
  store i32 %119, i32* %121, align 4
  %122 = load %struct.CellField*, %struct.CellField** %5, align 8
  %123 = getelementptr inbounds %struct.CellField, %struct.CellField* %122, i32 0, i32 0
  %124 = load i32, i32* %10, align 4
  %125 = sext i32 %124 to i64
  %126 = getelementptr inbounds [720 x [1280 x i32]], [720 x [1280 x i32]]* %123, i64 0, i64 %125
  %127 = load i32, i32* %12, align 4
  %128 = sext i32 %127 to i64
  %129 = getelementptr inbounds [1280 x i32], [1280 x i32]* %126, i64 0, i64 %128
  %130 = load i32, i32* %129, align 4
  %131 = load i32*, i32** %6, align 8
  %132 = getelementptr inbounds i32, i32* %131, i64 7
  store i32 %130, i32* %132, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @CellField_IsCellDead(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = load i32, i32* @CellField_dead_cell, align 4
  %5 = icmp eq i32 %3, %4
  %6 = zext i1 %5 to i32
  ret i32 %6
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn writeonly }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
