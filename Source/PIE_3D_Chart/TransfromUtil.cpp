// Fill out your copyright notice in the Description page of Project Settings.


#include "TransfromUtil.h"
#include "Engine/EngineTypes.h"
#include "Math/Vector.h"
#include "Components/SceneComponent.h"
#include "Math/IntRect.h"
#include "SceneView.h"
#include "Kismet/GameplayStatics.h"

#define Cos(A) FMath::Cos(FMath::DegreesToRadians(A))
#define Sin(A) FMath::Sin(FMath::DegreesToRadians(A))
#define Tan(A) FMath::Tan(FMath::DegreesToRadians(A))

FMatrix UTransfromUtil::RotatorToMatrix(FRotator rotation)
{
	float SP = Sin(rotation.Pitch);
	float CP = Cos(rotation.Pitch);
	float SY = Sin(rotation.Yaw);
	float CY = Cos(rotation.Yaw);
	float SR = Sin(rotation.Roll);
	float CR = Cos(rotation.Roll);

	FMatrix matrix;

	matrix.M[0][0] = (CP * CY);
	matrix.M[0][1] = (CP * SY);
	matrix.M[0][2] = (SP);
	matrix.M[0][3] = 0;

	matrix.M[1][0] = (SR * SP * CY - CR * SY);
	matrix.M[1][1] = (SR * SP * SY + CR * CY);
	matrix.M[1][2] = (-SR * CP);
	matrix.M[1][3] = 0;

	matrix.M[2][0] = (-(CR * SP * CY + SR * SY));
	matrix.M[2][1] = (CY * SR - CR * SP * SY);
	matrix.M[2][2] = (CR * CP);
	matrix.M[2][3] = 0;

	matrix.M[3][0] = 0;
	matrix.M[3][1] = 0;
	matrix.M[3][2] = 0;
	matrix.M[3][3] = 1;

	return matrix;
}

FVector2D UTransfromUtil::WorldToScreen(FVector worldLocation, FMinimalViewInfo camViewInfo, FVector2D ScreenSize, USceneComponent* camera)
{
	if (!camera) return FVector2D();

	FVector vAxisX = camera->GetForwardVector();
	FVector vAxisY = camera->GetRightVector();
	FVector vAxisZ = camera->GetUpVector();

	FVector vDelta;
	FVector vTransformed;
	FVector2D result;

	vDelta.X = worldLocation.X - camViewInfo.Location.X;
	vDelta.Y = worldLocation.Y - camViewInfo.Location.Y;
	vDelta.Z = worldLocation.Z - camViewInfo.Location.Z;

	vTransformed.X = FVector::DotProduct(vDelta, vAxisY);//horizontal
	vTransformed.Y = FVector::DotProduct(vDelta, vAxisZ);//vertical
	vTransformed.Z = FVector::DotProduct(vDelta, vAxisX);//depth

	if (vTransformed.Z < 1.0f)
	{
		vTransformed.Z = 1.0f;
	}

	float fov = camViewInfo.FOV;
	float screenCenterX = ScreenSize.X / 2;
	float screenCenterY = ScreenSize.Y / 2;

	// Homogeneous w 
	// (ScreenSize.X/2) / w = tan(fov/2)
	float w = screenCenterX / Tan(0.5 * fov);

	// offsetX / vTransformed.X = w / vTransformed.Z 
	// offsetY / vTransformed.Y = w / vTransformed.Z 
	result.X = (screenCenterX + vTransformed.X * w / vTransformed.Z);
	result.Y = (screenCenterY - vTransformed.Y * w / vTransformed.Z);

	return result;
}

bool UTransfromUtil::ScreenToWorld(FVector2D screenPos, FMinimalViewInfo camViewInfo, FVector2D ScreenSize, USceneComponent* camera, FVector& WorldOrigin, FVector& WorldDirection)
{
	if (!camera) return false;

	FMatrix view;
	FMatrix projection;
	FMatrix viewProjection;

	UGameplayStatics::GetViewProjectionMatrix(camViewInfo, view, projection, viewProjection);
	FSceneView::DeprojectScreenToWorld(screenPos, FIntRect(FIntPoint(0, 0), FIntPoint(ScreenSize.X, ScreenSize.Y)), viewProjection.InverseFast(), WorldOrigin, WorldDirection);
	return true;
}

