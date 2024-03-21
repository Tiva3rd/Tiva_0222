// Fill out your copyright notice in the Description page of Project Settings.


#include "OJS/RoomInfoWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OJS/TivaNetGameInstance.h"

void URoomInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_join->OnClicked.AddDynamic( this , &URoomInfoWidget::OnMyJoinRoom );
}

void URoomInfoWidget::setInfo(const FRoomInfo& info)
{
	roomIndex = info.index;

	txt_roomName->SetText( FText::FromString( info.roomName ) );
	txt_hostName->SetText( FText::FromString( info._hostName ) );
	txt_playerCount->SetText( FText::FromString( info.playerCount ) );
	txt_ping->SetText( FText::FromString( info.pingMS ) );
}

void URoomInfoWidget::OnMyJoinRoom()
{
	auto ti = Cast<UTivaNetGameInstance>( GetWorld()->GetGameInstance() );
	if (ti)
	{
		ti->JoinRoom( roomIndex );
	}
}

