// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY()
	class UTivaNetGameInstance* ti;

public:
	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UEditableText* edit_roomName;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class USlider* slider_maxPlayer;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UTextBlock* text_maxPlayer;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_doCreateRoom;

	// slider_maxPlayer를 이동시키면 text_maxPlayer의 값도 바뀌게 하고싶다.
	UFUNCTION()
	void OnMyValueChage_maxPlayer( float value );

	// 버튼을 누르면 UNetGameInstance::CreateRoom 를 호출하고싶다.
	UFUNCTION()
	void OnMyClicked_doCreateRoom();


	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UWidgetSwitcher* WidgetSwitcherLobby;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UButton* btn_goCreateRoom;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UButton* btn_goFindRoom;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UButton* btn_goMenuFromCreateRoom;

	UPROPERTY( EditAnywhere , meta = (BindWidget) )
	class UButton* btn_goMenuFromFindRoom;

	void SwitchPanel( int32 index );

	UFUNCTION()
	void OnGoMenu();

	UFUNCTION()
	void OnGoCreateRoom();

	UFUNCTION()
	void OnGoFindRoom();

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UScrollBox* scroll_RoomList;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<class URoomInfoWidget> roomInfoFactory;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_doFindRoomList;

	UFUNCTION()
	void OnMyDoFindRoomList();

	UFUNCTION()
	void AddRoomInfoWidget( const struct FRoomInfo& roomInfo );


};
