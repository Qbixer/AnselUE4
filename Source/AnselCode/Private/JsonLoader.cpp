// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonLoader.h"
#include "JsonUtilities.h"

// Sets default values
AJsonLoader::AJsonLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonLoader::BeginPlay()
{
	Super::BeginPlay();
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/pd_map_full.data";
	FString JsonString; //Json converted to FString

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);


	//Create a json object to store the information from the json string
	//The json reader is used to deserialize the json object later on
	TSharedPtr<FJsonValue> JsonTable;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonTable)) {
		//The person "object" that is retrieved from the given json file
		auto objects = JsonTable->AsArray();
		for (int i = 0;i < objects.Num();i++) {
			auto object = objects[i]->AsObject();			
			GLog->Log(object->GetStringField("type"));
		}

		GLog->Log("deserialize completed");
	

	} else {
		GLog->Log("couldn't deserialize");
	}
	
}

// Called every frame
void AJsonLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

