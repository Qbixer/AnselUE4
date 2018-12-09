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
	TMap<FString, UInstancedStaticMeshComponent*> instancedMeshes;
	
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/pd_map_full.data";
	FString JsonString; //Json converted to FString

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);

	UStaticMesh* Asset = nullptr;
	
	//Create a json object to store the information from the json string
	//The json reader is used to deserialize the json object later on
	TSharedPtr<FJsonValue> JsonTable;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	int j = 0;

	if (FJsonSerializer::Deserialize(JsonReader, JsonTable)) {
		//The person "object" that is retrieved from the given json file
		auto objects = JsonTable->AsArray();
		for (int i = 0;i < objects.Num();i++) {
			auto object = objects[i]->AsObject();
			if (!instancedMeshes.Contains(object->GetStringField("type"))) {
				if (j < meshes.Num()) {
					UInstancedStaticMeshComponent *ISMComp = NewObject<UInstancedStaticMeshComponent>(this);
					ISMComp->RegisterComponent();
					ISMComp->SetStaticMesh(meshes[j]);
					ISMComp->SetFlags(RF_Transactional);
					this->AddInstanceComponent(ISMComp);
					instancedMeshes.Add(object->GetStringField("type"), ISMComp);
					j++;
				} else {
					GLog->Log(object->GetStringField("type"));
					continue;
				}
			}		
			auto bounds = object->GetObjectField("bounds");
			auto x = bounds->GetNumberField("x");
			auto y = bounds->GetNumberField("y");
			auto height = bounds->GetNumberField("height");
			auto width = bounds->GetNumberField("width");
			int z;
			if (!object->TryGetNumberField("hierarchyVisibilityLevel", z))
				z = -1;
			FTransform transform = FTransform(FQuat::Identity, FVector(x, y, z), FVector(width/100, height/100, 1.0f));
			instancedMeshes[object->GetStringField("type")]->AddInstance(transform);

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

