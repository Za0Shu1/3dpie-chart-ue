// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TransfromUtil.generated.h"

/**
 * 
 */
UCLASS()
class PIE_3D_CHART_API UTransfromUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PaintUtil")
		static FVector2D WorldToScreen(FVector worldLocation, FMinimalViewInfo camViewInfo, FVector2D ScreenSize, USceneComponent* camera);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PaintUtil")
		static bool ScreenToWorld(FVector2D screenPos, FMinimalViewInfo camViewInfo, FVector2D ScreenSize, USceneComponent* camera, FVector& WorldOrigin, FVector& WorldDirection);

private:
	static FMatrix RotatorToMatrix(FRotator rotation);
};
