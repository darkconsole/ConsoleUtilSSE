#include "Papyrus.h"

#include <cstdlib>  // size_t
#include <string>  // string

#include "RE/Skyrim.h"


namespace ConsoleUtil
{
	enum
	{
		kVersion = 1
	};


	void ExecuteCommand(RE::StaticFunctionTag*, RE::BSFixedString a_command)
	{
		// 41 54 41 56 41 57 48 83 EC 40 48 C7 44 24 38 FE FF FF FF 48 89 5C 24 60 48 89 6C 24 68 48 89 74 24 70 48 89 7C 24 78 4D 8B F1
		using func_t = void(RE::Script*, RE::ConsoleManager*, std::size_t, void*);
		RelocUnrestricted<func_t*> func(0x002E75F0);	// 1_5_73

		auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
		if (!scriptFactory) {
			return;
		}

		auto script = scriptFactory->Create();
		if (!script) {
			return;
		}

		std::string cmd = a_command.c_str();
		script->unk38 = cmd.data();
		func(script, RE::ConsoleManager::GetSingleton(), 1, 0);
		script->unk38 = 0;
		delete script;
	}


	RE::TESObjectREFR* GetSelectedReference(RE::StaticFunctionTag*)
	{
		auto handle = RE::Console::GetSelectedRef();
		RE::TESObjectREFRPtr selectedRef;
		RE::TESObjectREFR::LookupByHandle(handle, selectedRef);
		return selectedRef.get();
	}


	void SetSelectedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_reference)
	{
		auto handle = a_reference ? a_reference->GetOrCreateRefHandle() : *g_invalidRefHandle;

		auto mm = RE::MenuManager::GetSingleton();
		auto uiStrHolder = RE::UIStringHolder::GetSingleton();
		if (!mm || !uiStrHolder) {
			return;
		}

		auto console = mm->GetMenu<RE::Console>(uiStrHolder->console);
		if (console) {
			console->SetSelectedRef(handle);
		}
	}


	RE::BSFixedString ReadMessage(RE::StaticFunctionTag*)
	{
		auto consoleManager = RE::ConsoleManager::GetSingleton();
		return consoleManager ? consoleManager->lastMessage : "";
	}


	void PrintMessage(RE::StaticFunctionTag*, RE::BSFixedString a_message)
	{
		auto consoleManager = RE::ConsoleManager::GetSingleton();
		if (consoleManager) {
			consoleManager->Print(a_message.c_str());
		}
	}


	SInt32 GetVersion(RE::StaticFunctionTag*)
	{
		return kVersion;
	}


	bool RegisterFuncs(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("ExecuteCommand", "ConsoleUtil", ExecuteCommand);
		a_vm->RegisterFunction("GetSelectedReference", "ConsoleUtil", GetSelectedReference);
		a_vm->RegisterFunction("SetSelectedReference", "ConsoleUtil", SetSelectedReference);
		a_vm->RegisterFunction("ReadMessage", "ConsoleUtil", ReadMessage);
		a_vm->RegisterFunction("PrintMessage", "ConsoleUtil", PrintMessage);
		a_vm->RegisterFunction("GetVersion", "ConsoleUtil", GetVersion);
		return true;
	}
}
