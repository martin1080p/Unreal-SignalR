/*
 * MIT License
 *
 * Copyright (c) 2020-2022 Frozen Storm Interactive, Yoann Potinet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "CoreMinimal.h"
#include "IHubConnection.h"
#include "UObject/Object.h"
#include "SignalRValueWrapper.h"
#include "SignalRHubConnectionWrapper.generated.h"

struct FSignalRValueWrapper;

UCLASS(BlueprintType, DisplayName = "SignalR Hub Connection")
class SIGNALRBLUEPRINT_API USignalRHubConnectionWrapper : public UObject
{
    GENERATED_BODY()
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHubConnectedEvent);
    UPROPERTY(BlueprintAssignable)
    FOnHubConnectedEvent OnHubConnected;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHubConnectionErrorEvent, const FString&, Error);
    UPROPERTY(BlueprintAssignable)
    FOnHubConnectionErrorEvent OnHubConnectionError;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHubConnectionClosedEvent);
    UPROPERTY(BlueprintAssignable)
    FOnHubConnectionClosedEvent OnHubConnectionClosed;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHubEventMessage, const FString&, EventName, const TArray<FSignalRValueWrapper>&, Arguments);
    UPROPERTY(BlueprintAssignable)
    FOnHubEventMessage OnHubEventMessage;

    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSignalREvent, const TArray<FSignalRValueWrapper>&, Arguments);

    void SetHubConnection(const TSharedPtr<IHubConnection>& HubConnection);

    UFUNCTION(BlueprintCallable, Category = "SignalR")
    void Start();

    UFUNCTION(BlueprintCallable, Category = "SignalR")
    void Stop();

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "Invoke SignalR Method", Meta = (AutoCreateRefTerm = "Arguments"))
    void Send(const FString& EventName, const TArray<FSignalRValueWrapper>& Arguments);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInvokeCompleted, const FSignalRInvokeResultWrapper&, Result);

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "Invoke SignalR Method (Latent)", Meta = (AutoCreateRefTerm = "Arguments, OnCompleted"))
    void Invoke(const FString& EventName, const TArray<FSignalRValueWrapper>& Arguments, const FOnInvokeCompleted& OnCompleted);

    UFUNCTION(BlueprintCallable, Category = "SignalR", DisplayName = "On Event Message", Meta = (AutoCreateRefTerm = "EventName, OnEvent"))
    void BindEvent(const FString& EventName, const FOnSignalREvent& OnEvent);

private:
    UPROPERTY()
    TMap<FString, FOnSignalREvent> RegisteredEvents;

    void OnInvokeCompleted(const FSignalRInvokeResult& Result, FOnInvokeCompleted Delegate);

    void BroadcastOnHubConnected();
    void BroadcastOnHubConnectionError(const FString& Error);
    void BroadcastOnHubConnectionClosed();

    TSharedPtr<IHubConnection> HubConnection;
};
