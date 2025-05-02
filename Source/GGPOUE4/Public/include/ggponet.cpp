/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "ggponet.h"
#include "UObject/UObjectGlobals.h"

#define ARRAYSIZE(a) sizeof(a) / sizeof(a[0])

//UGGPONetworkAddress

UGGPONetworkAddress* UGGPONetworkAddress::CreateNetworkAddress(UObject* Outer, const FName Name, const FString Address)
{
    UGGPONetworkAddress* Result = NewObject<UGGPONetworkAddress>(Outer, Name);
    // Same type, apparently?
    const wchar_t* address = *Address;

    wchar_t WideIpBuffer[128];
    uint32 WideIpBufferSize = (uint32)ARRAYSIZE(WideIpBuffer);
    // Check and get port
    if (swscanf_s(address, L"%[^:]:%hd", WideIpBuffer, WideIpBufferSize, &Result->Port) != 2) {
        Result->bValidAddress = false;
    }
    else
    {
        // Get address
        wcstombs_s(nullptr, Result->IpAddress, ARRAYSIZE(Result->IpAddress), WideIpBuffer, _TRUNCATE);
    }

    return Result;
}
UGGPONetworkAddress* UGGPONetworkAddress::CreateLocalAddress(UObject* Outer, const FName Name, int32 LocalPort)
{
    UGGPONetworkAddress* Result = NewObject<UGGPONetworkAddress>(Outer, Name);

    Result->bValidAddress = true;
    Result->Port = (uint16)LocalPort;
    strcpy(Result->IpAddress, "127.0.0.1");

    return Result;
}

void UGGPONetworkAddress::GetIpAddress(char OutAddress[32]) const
{
    std::memcpy(OutAddress, IpAddress, sizeof(IpAddress));
}

bool UGGPONetworkAddress::IsValidAddress() const
{
    return bValidAddress;
}
FString UGGPONetworkAddress::GetIpAddressString() const
{
    auto address = FString(ANSI_TO_TCHAR(IpAddress));
    return address;
}
int32 UGGPONetworkAddress::GetPort() const
{
    return Port;
}

bool UGGPONetworkAddress::IsSameAddress(const UGGPONetworkAddress* Other) const
{
    if (bValidAddress != Other->bValidAddress)
        return false;
    if (!std::equal(std::begin(IpAddress), std::end(IpAddress), std::begin(Other->IpAddress)))
        return false;
    if (Port != Other->Port)
        return false;

    return true;
}

// UGGPONetwork

UGGPONetwork* UGGPONetwork::CreateNetwork(UObject* Outer, const FName Name, int32 InNumPlayers, int32 PlayerIndex, int32 InLocalPort, TArray<FString> RemoteAddresses)
{
    UGGPONetwork* Result = NewObject<UGGPONetwork>(Outer, Name);

    Result->LocalPlayerIndex = PlayerIndex - 1;
    Result->NumPlayers = InNumPlayers;

    if (Result->LocalPlayerIndex >= 0)
    {
        int32 remoteIndex = 0;
        // Add remote players
        for (int32 i = 0; i < InNumPlayers; i++)
        {
            // Only the port matters for local player
            if (i == Result->LocalPlayerIndex)
            {
                // Create a GGPO Network Address and add to the addresses
                UGGPONetworkAddress* address = UGGPONetworkAddress::CreateLocalAddress(
                    Outer,
                    FName(FString::Printf(TEXT("P%dIPAddress"), i + 1)),
                    InLocalPort);
                Result->Addresses.Add(address);
            }
            else
            {
                // If we ran out of remote addresses, clear the addresses and break
                if (remoteIndex >= RemoteAddresses.Num())
                {
                    Result->Addresses.Empty();
                    break;
                }

                // Create a GGPO Network Address and add to the addresses
                UGGPONetworkAddress* address = UGGPONetworkAddress::CreateNetworkAddress(
                    Outer,
                    FName(FString::Printf(TEXT("P%dIPAddress"), i + 1)),
                    RemoteAddresses[remoteIndex]);
                Result->Addresses.Add(address);
                remoteIndex++;
            }
        }

        // Add any spectators
        if (Result->Addresses.Num() > 0)
        {
            for (int32 i = remoteIndex; i < RemoteAddresses.Num(); i++)
            {
                // Create a GGPO Network Address and add to the spectators
                UGGPONetworkAddress* address = UGGPONetworkAddress::CreateNetworkAddress(
                    Outer,
                    FName(FString::Printf(TEXT("S%dIPAddress"), i + 1 - remoteIndex)),
                    RemoteAddresses[i]);
                Result->Spectators.Add(address);
            }
        }
    }
    // Spectator
    else
    {
        if (RemoteAddresses.Num() >= 1)
        {
            // Create a GGPO Network Address and add to the addresses
            UGGPONetworkAddress* address = UGGPONetworkAddress::CreateNetworkAddress(
                Outer,
                FName(FString::Printf(TEXT("HostIPAddress"))),
                RemoteAddresses[0]);
            Result->Addresses.Add(address);
        }

        Result->LocalPort = InLocalPort;
    }

    return Result;
}

bool UGGPONetwork::AllValidAddresses() const
{
    // If there are no players, this isn't valid
    if (Addresses.Num() == 0)
        return false;

    for (int32 i = 0; i < Addresses.Num(); i++)
    {
        // If an address is invalid, return false
        UGGPONetworkAddress* address = Addresses[i];
        if (!address->IsValidAddress())
            return false;
    }

    for (int32 i = 0; i < Spectators.Num(); i++)
    {
        // If an address is invalid, return false
        UGGPONetworkAddress* address = Spectators[i];
        if (!address->IsValidAddress())
            return false;
    }

    return AllUniqueAddresses();
}
bool UGGPONetwork::AllUniqueAddresses() const
{
    // Compare both remote players and spectators
    auto AllRemotes = Addresses;
    AllRemotes.Append(Spectators);

    for (int32 i = 0; i < AllRemotes.Num(); i++)
    {
        for (int32 j = i + 1; j < AllRemotes.Num(); j++)
        {
            // If the address is the same, return false
            if (AllRemotes[i]->IsSameAddress(AllRemotes[j]))
                return false;
        }
    }

    return true;
}

UGGPONetworkAddress* UGGPONetwork::GetAddress(int32 Index) const
{
    if (Index < 0 || Index >= Addresses.Num())
        return nullptr;

    return Addresses[Index];
}
int32 UGGPONetwork::NumAddresses() const
{
    return Addresses.Num();
}

UGGPONetworkAddress* UGGPONetwork::GetSpectator(int32 Index) const
{
    if (Index < 0 || Index >= Spectators.Num())
        return nullptr;

    return Spectators[Index];
}
int32 UGGPONetwork::NumSpectators() const
{
    return Spectators.Num();
}

int32 UGGPONetwork::GetLocalPort() const
{
    // For spectators
    if (LocalPlayerIndex <= -1)
        return LocalPort;

    return Addresses[LocalPlayerIndex]->GetPort();
}

