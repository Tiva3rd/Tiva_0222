// Fill out your copyright notice in the Description page of Project Settings.


#include "OJS/TivaNetGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

void UTivaNetGameInstance::Init()
{
	Super::Init();
	if(auto subsystem = IOnlineSubsystem::Get())
	{
		sessionInterface = subsystem->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject( this , &UTivaNetGameInstance::OnMyCreateRoomComplete );
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject( this , &UTivaNetGameInstance::OnMyFindOtherRoomsComplete );
	}

	FTimerHandle handle;
	GetTimerManager().SetTimer( handle , [&]()
	{
		FindOtherRooms();
	},5,false );

}

void UTivaNetGameInstance::CreateRoom(int maxPlayerCount, FString roomName)
	{

	FOnlineSessionSettings setting;

	// 1. 데디케이트 서버인가??
	setting.bIsDedicated = false;
	// 2. 랜선인가?
	auto subSys = IOnlineSubsystem::Get();
	setting.bIsLANMatch = subSys->GetSubsystemName().IsEqual( "NULL" );
	// 3. 공개로 입장할 수 있는가? 아니면 친구초대로만???
	setting.bShouldAdvertise = true;
	// 4. 온라인 상태(presence)를 공개적으로 사용할것인가? -> ping정보
	setting.bUsesPresence = true;
	// 5. 중간입장이 가능한가?
	setting.bAllowJoinInProgress = true;
	setting.bAllowJoinViaPresence = true;
	// 6. 최대 입장 가능한 수 설정
	setting.NumPublicConnections = maxPlayerCount;
	// 7. 커스텀 정보 설정
	setting.Set( TEXT( "HOST_NAME" ) , myNickName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	setting.Set( TEXT( "ROOM_NAME" ) , roomName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	// 8. netID 찾기
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG( LogTemp , Warning , TEXT( "CreateRoom Start!!! roomNamd : %s, netID : %s" ) , *roomName , *netID->ToString() );

	sessionInterface->CreateSession( *netID , FName( *roomName ) , setting );

		//FOnlineSessionSettings setting;

		//// 1. 데디케이트 서버인가?
		//setting.bIsDedicated = false;
		//// 2. 랜선인가?
		//auto subSys = IOnlineSubsystem::Get();
		//setting.bIsLANMatch = subSys->GetSubsystemName().IsEqual( "NULL" );
		//// 3. 공개로 입장할 수 있는가? 아니면 친구초대로만?
		//setting.bShouldAdvertise = true;
		//// 4. 온라인 상태를 공개적으로 사용할것인가? -> ping 정보
		//setting.bUsesPresence = true;
		//// 5. 중간 입장이 가능한가?
		//setting.bAllowJoinInProgress = true;
		//setting.bAllowJoinViaPresence = true;
		//// 6. 최대 입장 가능한 수 설정
		//setting.NumPublicConnections = maxPlayerCount;
		//// 7. 커스텀정보 설정
		//setting.Set( TEXT( "HOST_NAME" ) , myNickName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
		//setting.Set( TEXT( "ROOM_NAME" ) , roomName , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );

		//// 8. netID 찾기
		//FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		//UE_LOG( LogTemp , Warning , TEXT( "CreateRoomName : %s, netID : %s" ) , *roomName , *netID->ToString() );

		//sessionInterface->CreateSession( *netID , FName( *roomName ) , setting );
	}

void UTivaNetGameInstance::OnMyCreateRoomComplete( FName sessionName , bool bWasSuccessful )
{
	UE_LOG( LogTemp , Warning , TEXT( "OnCreateRoomComplete -> sessionName : %s, bwasSuccessful : %d" ) , *sessionName.ToString() , bWasSuccessful );

		//if (bWasSuccessful)
		//{
		//	// 입장한 방의 이름을 기억하고싶다.
		//	myRoomName = sessionName.ToString();
		//	// 서버는 세계 여행을 떠나고싶다. 어디로???
		//	FString url = TEXT( "/Game/Net/Maps/BattleMap?listen" );
		//	GetWorld()->ServerTravel( url );
		//}
}

void UTivaNetGameInstance::FindOtherRooms()
{
	// 1. FOnlineSessionSearch객체를 생성
	roomSearch = MakeShareable( new FOnlineSessionSearch() );
	// 2. 세션 검색 조건 설정
	roomSearch->QuerySettings.Set( SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals );
	// 3. 최대 검색 갯수를 정하고 싶다.
	roomSearch->MaxSearchResults = 10;
	// 4. 랜선인지 아닌지를 정하고 싶다.
	auto subSys = IOnlineSubsystem::Get();
	roomSearch->bIsLanQuery = subSys->GetSubsystemName().IsEqual( "NULL" );


	// 5. 검색을 하고싶다.
	sessionInterface->FindSessions( 0 , roomSearch.ToSharedRef() );
	UE_LOG( LogTemp , Warning , TEXT( "------FindSessionDONE!------" ) );
}

void UTivaNetGameInstance::OnMyFindOtherRoomsComplete(bool bWasSuccessful)
{
	int32 int_reuslt = roomSearch->SearchResults.Num();
	UE_LOG( LogTemp , Warning , TEXT( "OnMyFindOtherRoomsComplete7 : %d" ) , bWasSuccessful );
	UE_LOG( LogTemp , Warning , TEXT("방 개수 : %d"), int_reuslt );

	for (int i = 0; i < roomSearch->SearchResults.Num(); i++)
	{
		auto r = roomSearch->SearchResults[i];
		if (false == r.IsValid())
			continue;

		FRoomInfo info;

		info.index = i;

		r.Session.SessionSettings.Get( TEXT( "ROOM_NAME" ) , info.roomName );
		r.Session.SessionSettings.Get( TEXT( "HOST_NAME" ) , info._hostName );

		int32 max = r.Session.SessionSettings.NumPublicConnections; // 최대인원수
		// 현재 입장 플레이어 수 = 최대 - 입장가능 수 
		int32 current = max - r.Session.NumOpenPublicConnections;
		info.playerCount = FString::Printf( TEXT( "%d/%d" ) , current , max );
		info.pingMS = FString::Printf( TEXT( "%dms" ) , r.PingInMs );
		info.PrintLog();

		if (onAddRoomInfoDelegate.IsBound())
		{
			onAddRoomInfoDelegate.Broadcast( info );
		}
	}
}
