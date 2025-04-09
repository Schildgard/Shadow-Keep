// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCSaveDataInfoBase.h"

FNPCSaveDataInfoBase::FNPCSaveDataInfoBase()
{
	bIsAlive = true;
}

void FNPCSaveDataInfoBase::UpdateNPCSaveData(bool CurrentStatus)
{
	bIsAlive = CurrentStatus;
}
