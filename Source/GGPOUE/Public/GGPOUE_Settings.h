// Copyright 2020 BwdYeti.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "log.h"
#include "GGPOUE_Settings.generated.h"

/**
 * 
 */
UCLASS(Config = "GGPOSettings", meta = (DisplayName = "GGPO"))
class GGPOUE_API UGGPOUE_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGGPOUE_Settings(const FObjectInitializer& obj);

	UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "Logging")
		bool LoggingEnabled = false;

	UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "Logging")
		EGGPOLogVerbosity LogVerbosity = EGGPOLogVerbosity::Info;
	
};
