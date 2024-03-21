// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIVA_API URoomInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UTextBlock* txt_roomName;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UTextBlock* txt_hostName;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UTextBlock* txt_playerCount;

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UTextBlock* txt_ping;

	int32 roomIndex;

	//void setInfo( int32 idx , FString roomName , FString hostName , FString playerCount , FString ping );
	void setInfo( const struct FRoomInfo& info );

	UPROPERTY( EditDefaultsOnly , meta = (BindWidget) )
	class UButton* btn_join;

	UFUNCTION()
	void OnMyJoinRoom();
};
