// Copyright 2020 BwdYeti.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGPOGameInstance.generated.h"

// Forward declarations
class UGGPONetwork;

/**
 * 
 */
UCLASS()
class GGPOUE_API UGGPOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UGGPONetwork> NetworkAddresses;

    /// <summary>
    /// Creates a collection of network addresses.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "GGPO")
        void CreateNetwork(int32 NumPlayers, int32 PlayerIndex, int32 LocalPort, TArray<FString> RemoteAddresses, TArray<FString> SpectatorAddresses);

    /// <summary>
    /// Creates a collection of network addresses for a spectator.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "GGPO")
        void CreateSpectatorNetwork(int32 NumPlayers, int32 LocalPort, FString HostAddress);

};
