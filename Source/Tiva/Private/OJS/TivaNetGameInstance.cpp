// Fill out your copyright notice in the Description page of Project Settings.


#include "OJS/TivaNetGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UTivaNetGameInstance::Init()
{
	Super::Init();
	if(auto subsystem = IOnlineSubsystem::Get())
	{
		sessionInterface = subsystem->GetSessionInterface();
	}

	FTimerHandle handle;
	GetTimerManager().SetTimer( handle , [&]()
	{
		CreateRoom( 10 , hostName );
	},5,false );

}

void UTivaNetGameInstance::CreateRoom(int maxPlayerCount, FString roomName)
	{
	UE_LOG( LogTemp , Warning , TEXT( "CreateRooom!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ) );

		FOnlineSessionSettings setting;

		// 1. 데디케이트 서버인가?
		setting.bIsDedicated = false;
		// 2. 랜선인가?
		auto subSys = IOnlineSubsystem::Get();
		setting.bIsLANMatch = subSys->GetSubsystemName().IsEqual( "NULL" );
		// 3. 공개로 입장할 수 있는가? 아니면 친구초대로만?
		setting.bShouldAdvertise = true;
		// 4. 온라인 상태를 공개적으로 사용할것인가? -> ping 정보
		setting.bUsesPresence = true;
		// 5. 중간 입장이 가능한가?
		setting.bAllowJoinInProgress = true;
		setting.bAllowJoinViaPresence = true;
		// 6. 최대 입장 가능한 수 설정
		setting.NumPublicConnections = maxPlayerCount;
		// 7. 커스텀정보 설정
		setting.Set( TEXT( "HOST_NAME" ) , hostName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
		setting.Set( TEXT( "ROOM_NAME" ) , roomName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );

		// 8. netID 찾기
		FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		UE_LOG( LogTemp , Warning , TEXT( "CreateRoomName : %s, netID : %s" ) , *roomName , *netID->ToString() );

		sessionInterface->CreateSession( *netID , FName( *roomName ) , setting );

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject( this , &UTivaNetGameInstance::OnMyCreateRoom );
	}

void UTivaNetGameInstance::OnMyCreateRoom( FName sessionName , bool bWasSuccessful )
{
	UE_LOG( LogTemp , Warning , TEXT( "OnMyCreateRoomComplete : %s, bWasSuccessful : %d" ) , *sessionName.ToString() , bWasSuccessful );
}
