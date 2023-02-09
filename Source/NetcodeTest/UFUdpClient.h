// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UFUdpClient.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NETCODETEST_API UFUdpClient : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UDPSocketConnect")
		bool Connect(const FString& RemoteAddress, const int32 RemotePort, FString& ErrorMessage);


	UFUNCTION(BlueprintCallable, Category = "UDPSocketConnect")
		bool SendVectorData(const FVector& Vector, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "UDPSocketConnect")
		bool SendData(const FString& SendData, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "UDPSocketConnect")
		bool ReceiveData(TArray<uint8>& RecvData, FString& ErrorMessage);

private:
	FSocket* Socket;
};
