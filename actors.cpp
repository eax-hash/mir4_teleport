#include "inc.h"

std::vector<CG::AMM_NetPC*> unreal::c_actors::get_actors()
{
	std::vector<CG::AMM_NetPC*> actors;

	auto UWorld = *(CG::UWorld**)(memory::c_offsets::get_instance()->m_GWorld);
	if (!UWorld || !UWorld->PersistentLevel) return actors;

	auto numActors = *(uint32_t*)((uintptr_t)UWorld->PersistentLevel + 0xA0);
	if (!numActors) return actors;

	auto arrayPtr = *(uintptr_t*)((uintptr_t)UWorld->PersistentLevel + 0x98);
	if (!arrayPtr) return actors;

	for (auto i = 0; i < numActors; i++)
	{
		auto actor = *(CG::AMM_NetPC**)(arrayPtr + (i * 0x8));
		if (!actor) continue;

		if (!actor->IsA(CG::AMM_NetPC::StaticClass())) continue;

		actors.push_back(actor);
	}

	return actors;
}

std::vector<CG::AMM_GatherObject*> unreal::c_actors::get_gather_actors()
{
	std::vector<CG::AMM_GatherObject*> actors;

	auto UWorld = *(CG::UWorld**)(memory::c_offsets::get_instance()->m_GWorld);
	if (!UWorld || !UWorld->PersistentLevel) return actors;

	auto numActors = *(uint32_t*)((uintptr_t)UWorld->PersistentLevel + 0xA0);
	if (!numActors) return actors;

	auto arrayPtr = *(uintptr_t*)((uintptr_t)UWorld->PersistentLevel + 0x98);
	if (!arrayPtr) return actors;

	for (auto i = 0; i < numActors; i++)
	{
		auto actor = *(CG::AMM_GatherObject**)(arrayPtr + (i * 0x8));
		if (!actor) continue;

		if (!actor->IsA(CG::AMM_GatherObject::StaticClass())) continue;

		actors.push_back(actor);
	}

	return actors;
}

CG::AMM_MainPC* unreal::c_actors::get_main_pc()
{
	auto UWorld = *(CG::UWorld**)(memory::c_offsets::get_instance()->m_GWorld);
	if (!UWorld || !UWorld->PersistentLevel) return nullptr;

	auto numActors = *(uint32_t*)((uintptr_t)UWorld->PersistentLevel + 0xA0);
	if (!numActors) return nullptr;

	auto arrayPtr = *(uintptr_t*)((uintptr_t)UWorld->PersistentLevel + 0x98);
	if (!arrayPtr) return nullptr;

	for (auto i = 0; i < numActors; i++)
	{
		auto actor = *(CG::AMM_MainPC**)(arrayPtr + (i * 0x8));
		if (!actor) continue;

		if (!actor->IsA(CG::AMM_MainPC::StaticClass())) continue;

		return actor;
	}

	return nullptr;
}

void unreal::c_actors::teleport(CG::FVector curLoc)
{
	auto mainPC = get_main_pc();
	if (!mainPC) return;

	mainPC->SetActorEnableCollision(false);

	auto newHitResult = CG::FHitResult();
	mainPC->m_MovementComponent->SetMovementMode(CG::Engine_EMovementMode::MOVE_Flying, 0);

	auto quantizedObj = CG::FVector_NetQuantize();
	auto quantizedObjNormal = CG::FVector_NetQuantizeNormal();
	quantizedObj.X = curLoc.X;
	quantizedObj.Y = curLoc.Y;
	quantizedObj.Z = curLoc.Z + 400;

	quantizedObjNormal.X = curLoc.X;
	quantizedObjNormal.Y = curLoc.Y;
	quantizedObjNormal.Z = curLoc.Z + 400;

	newHitResult.Distance = -1.f;
	newHitResult.Location = quantizedObj;
	newHitResult.Time = mainPC->GetGameTimeSinceCreation();
	newHitResult.Normal = quantizedObjNormal;
	newHitResult.ImpactPoint = quantizedObj;
	newHitResult.ImpactNormal = quantizedObjNormal;

	newHitResult.bBlockingHit = true;

	mainPC->K2_SetActorLocation(curLoc, true, &newHitResult, true);
	mainPC->SetActorEnableCollision(true);
	mainPC->K2_NormalAttack();
	mainPC->K2_OnReset();

	//mainPC->m_MovementComponent->SetMovementMode(CG::Engine_EMovementMode::MOVE_None, 0);
}