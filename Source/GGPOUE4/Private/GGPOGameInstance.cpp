// Copyright 2020 BwdYeti.


#include "GGPOGameInstance.h"
#include "include/ggponet.h"

void UGGPOGameInstance::CreateNetwork(int32 NumPlayers, int32 PlayerIndex, int32 LocalPort, TArray<FString> RemoteAddresses, TArray<FString> SpectatorAddresses)
{
	// Add the spectators to the end of the remote players
	// CreateNetwork() will count out the NumPlayers,
	// and then handle the spectators after that
	RemoteAddresses.Append(SpectatorAddresses);
	UGGPONetwork* addresses = UGGPONetwork::CreateNetwork(
		this,
		FName(FString(TEXT("GGPONetwork"))),
		NumPlayers,
		PlayerIndex,
		LocalPort,
		RemoteAddresses);
	NetworkAddresses = addresses;
}

void UGGPOGameInstance::CreateSpectatorNetwork(int32 NumPlayers, int32 LocalPort, FString HostAddress)
{
	UGGPONetwork* addresses = UGGPONetwork::CreateNetwork(
		this,
		FName(FString(TEXT("GGPONetwork"))),
		NumPlayers,
		-1,
		LocalPort,
		{ HostAddress });
	NetworkAddresses = addresses;
}

