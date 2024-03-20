// Fill out your copyright notice in the Description page of Project Settings.


#include "OJS/LobbyWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "OJS/RoomInfoWidget.h"
#include "OJS/TivaNetGameInstance.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ti = GetWorld()->GetGameInstance<UTivaNetGameInstance>();

	if(ti){
		ti->onAddRoomInfoDelegate.AddDynamic( this , &ULobbyWidget::AddRoomInfoWidget );
	}

		slider_maxPlayer->OnValueChanged.AddDynamic( this , &ULobbyWidget::OnMyValueChage_maxPlayer );

		text_maxPlayer->SetText( FText::AsNumber( slider_maxPlayer->GetValue() ) );

		slider_maxPlayer->SetMinValue( 2 );
		slider_maxPlayer->SetMaxValue( 10 );
		slider_maxPlayer->SetStepSize( 1 );
		slider_maxPlayer->MouseUsesStep = true;

		btn_doCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyClicked_doCreateRoom );

		btn_goCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnGoCreateRoom );
		btn_goFindRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnGoFindRoom );
		btn_goMenuFromCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnGoMenu );
		btn_goMenuFromFindRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnGoMenu );

		btn_doFindRoomList->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyDoFindRoomList );;
}


void ULobbyWidget::OnMyClicked_doCreateRoom()
{
	FString roomName = edit_roomName->GetText().ToString();
	if (edit_roomName->GetText().IsEmpty())
	{
		UE_LOG( LogTemp , Warning , TEXT( "Write Your Room Name1" ) );
	}

	TArray<FString> badWords;
	badWords.Add( TEXT( "바보" ) );
	bool isBadWord = false;
	for (int i = 0; i < badWords.Num(); i++)
	{
		if (roomName.Contains( badWords[i] ))
		{
			UE_LOG( LogTemp , Warning , TEXT( "Write Your Room Name2" ) );
			return;
		}
	}

	UE_LOG( LogTemp , Warning , TEXT( "FUCk111111" ));
	int32 maxPlayer = slider_maxPlayer->GetValue();
	ti->CreateRoom( maxPlayer , roomName);
	UE_LOG( LogTemp , Warning , TEXT( "Fuck2222222" ));
}

void ULobbyWidget::SwitchPanel(int32 index)
{
	WidgetSwitcherLobby->SetActiveWidgetIndex( index );
}

void ULobbyWidget::OnGoMenu()
{
	SwitchPanel(0);
}

void ULobbyWidget::OnGoCreateRoom()
{
	SwitchPanel(1);
}

void ULobbyWidget::OnGoFindRoom()
{
	SwitchPanel(2);
}

void ULobbyWidget::OnMyDoFindRoomList()
{
	scroll_RoomList->ClearChildren();
	if (ti)
	{
		ti->FindOtherRooms();
	}
	UE_LOG( LogTemp , Warning , TEXT( "FIndDone" ) );
}

void ULobbyWidget::AddRoomInfoWidget(const FRoomInfo& roomInfo)
{
	if (nullptr == scroll_RoomList || nullptr == roomInfoFactory)
		return;

	// 위젯을 생성해서 roomInfoFactory
	auto ui = CreateWidget<URoomInfoWidget>( GetWorld() , roomInfoFactory );

	// scroll_roomList의 자식으로 붙이고싶다.
	if(ui)
	scroll_RoomList->AddChild( ui );
}

void ULobbyWidget::OnMyValueChage_maxPlayer( float value )
{
	text_maxPlayer->SetText( FText::AsNumber( value ) );
}
