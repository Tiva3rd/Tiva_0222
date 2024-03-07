// Fill out your copyright notice in the Description page of Project Settings.


#include "JWK/BossHPWidget.h"

#include "Components/ProgressBar.h"

void UBossHPWidget::SetBosstHP( int32 BosstHP , int32 BossMaxHP )
{
	BossHPBar->SetPercent( (float)BosstHP / BossMaxHP );
}
