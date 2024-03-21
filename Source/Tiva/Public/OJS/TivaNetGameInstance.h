// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TivaNetGameInstance.generated.h"

/**
 * 
 */
USTRUCT( BlueprintType )
struct FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly )
	int32 index;
	UPROPERTY( EditDefaultsOnly )
	FString roomName;
	UPROPERTY( EditDefaultsOnly )
	FString _hostName;
	UPROPERTY( EditDefaultsOnly )
	FString playerCount;
	UPROPERTY( EditDefaultsOnly )
	FString pingMS;

	FORCEINLINE void PrintLog() const
	{
		UE_LOG( LogTemp , Warning , TEXT( "RoomName : %s, HostName:%s, PlayerCount : %s, Ping : %s" ) , *roomName , *_hostName , *playerCount , *pingMS );
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAddRoomInfoDelegate , const FRoomInfo& , roomInfo );
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FFindingRoomsDelegate , bool , bActive );

UCLASS()
class TIVA_API UTivaNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	IOnlineSessionPtr sessionInterface;

	// 방검색 후 방생성에 관련 델리게이트
	FAddRoomInfoDelegate onAddRoomInfoDelegate;

	FString myNickName = TEXT( "JacobyiPlayer" );
	// 현재 입장한 방의 이름을 기억하고싶다.
	FString myRoomName = TEXT( "JacobyiRoom" );


	void CreateRoom( int32 maxPlayerCount , FString roomName );

	void OnMyCreateRoomComplete( FName sessionName , bool bWasSuccessful );

	//FString _hostName;
	//FString roomName;

	TSharedPtr<FOnlineSessionSearch> roomSearch;
	// 세션검색요청
	void FindOtherRooms();
	// 세션검색응답
	void OnMyFindOtherRoomsComplete( bool bWasSuccessful );
};
