// Fill out your copyright notice in the Description page of Project Settings.

#include "NewsFeedManager.h"
#include "NewsFeedData.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"


void UNewsFeedManager::LoadFromDataAsset(UNewsFeedData* DataAsset)
{
    if (DataAsset)
    {
        NewsItems = DataAsset->NewsItems;
        OnNewsUpdated.Broadcast(NewsItems);
    }
}

void UNewsFeedManager::LoadFromJSON(const FString& FilePath)
{
    FString JsonContent;
    if (FFileHelper::LoadFileToString(JsonContent, *FilePath))
    {
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);

        TArray<FNewsItem> ParsedItems;

        TSharedPtr<FJsonObject> RootObj;
        if (FJsonSerializer::Deserialize(Reader, RootObj) && RootObj.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>* NewsArray;
            if (RootObj->TryGetArrayField("news", NewsArray))
            {
                for (const TSharedPtr<FJsonValue>& Entry : *NewsArray)
                {
                    TSharedPtr<FJsonObject> Obj = Entry->AsObject();
                    if (!Obj.IsValid()) continue;

                    FNewsItem Item;
                    Item.ID = Obj->GetStringField("id");
                    Item.Title = FText::FromString(Obj->GetStringField("title"));
                    Item.Description = FText::FromString(Obj->GetStringField("description"));
                    Item.ImageURL = Obj->GetStringField("image_url");
                    Item.Timestamp = Obj->GetStringField("timestamp");

                    ParsedItems.Add(Item);
                }
            }
        }

        NewsItems = ParsedItems;
        OnNewsUpdated.Broadcast(NewsItems);
    }
}
