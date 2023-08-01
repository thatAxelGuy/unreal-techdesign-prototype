// Copyright Axel Woermann
#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, true);
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(World, Location, 15.f, FColor::Green, true, -1.f, 0);
#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
        DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Cyan, true, -1.f, 0); \
    }

#define DRAW_ARROW(StartLocation, EndLocation) if(GetWorld()) DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 120.f, FColor::Magenta, true, -1.f, 0, 5.f);
#define DRAW_3D_GIZMO(StartLocation, ForwardVector, RightVector, UpVector, Length) if(GetWorld()) \
{\
    DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + ForwardVector * Length, 120.f, FColor::Red, true, -1.f, 0, 5.f);\
    DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + RightVector * Length, 120.f, FColor::Green, true, -1.f, 0, 5.f);\
    DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + UpVector * Length, 120.f, FColor::Blue, true, -1.f, 0, 5.f);\
}
	