#include "Events.h"

#include "RE/Skyrim.h"


namespace Events
{
	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
	{
		static MenuOpenCloseEventHandler singleton;
		return &singleton;
	}


	auto MenuOpenCloseEventHandler::ReceiveEvent(RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		using Message = RE::UIMessage::Message;

		auto uiStrHolder = RE::UIStringHolder::GetSingleton();
		if (a_event->menuName == uiStrHolder->mainMenu) {
			auto uiManager = RE::UIManager::GetSingleton();
			uiManager->AddMessage(uiStrHolder->console, Message::kOpen, 0);
		} else if (a_event->menuName == uiStrHolder->console) {
			auto uiManager = RE::UIManager::GetSingleton();
			uiManager->AddMessage(uiStrHolder->console, Message::kClose, 0);
			auto mm = RE::MenuManager::GetSingleton();
			mm->GetMenuOpenCloseEventSource()->RemoveEventSink(this);
		}

		return EventResult::kContinue;
	}
}
