#pragma once

#include "RE/Skyrim.h"


namespace Papyrus
{
	class ConsoleUtil
	{
	public:
		enum
		{
			kVersion = 4
		};


		static void ExecuteCommand(RE::StaticFunctionTag*, RE::BSFixedString a_command);
		static RE::TESObjectREFR* GetSelectedReference(RE::StaticFunctionTag*);
		static void SetSelectedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_reference);
		static RE::BSFixedString ReadMessage(RE::StaticFunctionTag*);
		static void PrintMessage(RE::StaticFunctionTag*, RE::BSFixedString a_message);
		static SInt32 GetVersion(RE::StaticFunctionTag*);

		static bool Register(RE::BSScript::IVirtualMachine* a_vm);


		static constexpr char CLASS_NAME[] = "ConsoleUtil";
	};


	bool Register(RE::BSScript::IVirtualMachine* a_vm);
}
