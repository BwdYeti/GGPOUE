// Copyright 2020 BwdYeti.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GGPOUE4_Settings.generated.h"

/**
 * 
 */
UCLASS(Config = "GGPOSettings", meta = (DisplayName = "GGPO"))
class GGPOUE4_API UGGPOUE4_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGGPOUE4_Settings(const FObjectInitializer& obj);

	UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "Logging")
		bool LoggingEnabled = false;
	
};
