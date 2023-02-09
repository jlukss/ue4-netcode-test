// Fill out your copyright notice in the Description page of Project Settings.
#include "UFUdpClient.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

	bool UFUdpClient::Connect(const FString& RemoteAddress, const int32 RemotePort, FString& ErrorMessage)
	{
		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Dgram"), TEXT("UDP Socket"), false);
		if (!Socket)
		{
			ErrorMessage = "Failed to create socket.";
			return false;
		}

		TSharedRef<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		LocalAddr->SetPort(54321);
		bool bBindSuccess = Socket->Bind(*LocalAddr);
		if (!bBindSuccess)
		{
			ErrorMessage = "Failed to bind socket.";
			return false;
		}

		TSharedRef<FInternetAddr> RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool bValid = false;
		RemoteAddr->SetIp(*RemoteAddress, bValid);
		RemoteAddr->SetPort(RemotePort);
		bool bConnectSuccess = Socket->Connect(*RemoteAddr);
		if (!bConnectSuccess)
		{
			ErrorMessage = "Failed to connect to remote address.";
			return false;
		}

		return true;
	}

	bool UFUdpClient::SendVectorData(const FVector& Vector, FString& ErrorMessage)
	{
		int32 BytesSent = 0;
		TArray<uint8> SendDataBytes;
		uint8 floatBytes[4];
		memcpy(floatBytes, &Vector.X, sizeof(float));
		SendDataBytes.Append(floatBytes, sizeof(float));

		bool bSendSuccess = Socket->Send(SendDataBytes.GetData(), SendDataBytes.Num(), BytesSent);
		if (!bSendSuccess)
		{
			ErrorMessage = "Failed to send data.";
			return false;
		}

		return true;
	}

	bool UFUdpClient::SendData(const FString& SendData, FString& ErrorMessage)
	{
		int32 BytesSent = 0;
		TArray<uint8> SendDataBytes;
		SendDataBytes.Append((uint8*)TCHAR_TO_UTF8(*SendData), SendData.Len() + 1);
		bool bSendSuccess = Socket->Send(SendDataBytes.GetData(), SendDataBytes.Num(), BytesSent);
		if (!bSendSuccess)
		{
			ErrorMessage = "Failed to send data.";
			return false;
		}

		return true;
	}

	bool UFUdpClient::ReceiveData(TArray<uint8>& RecvData, FString& ErrorMessage)
	{
		RecvData.SetNumUninitialized(1024);
		int32 BytesRead = 0;
		TSharedRef<FInternetAddr> Sender = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool bRecvSuccess = Socket->RecvFrom(RecvData.GetData(), RecvData.Num(), BytesRead, *Sender);
		if (!bRecvSuccess)
		{
			ErrorMessage = "Failed to receive data.";
			return false;
		}

		RecvData.SetNum(BytesRead);
		return true;
	}
