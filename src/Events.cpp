#include "Events.h"

#include "RE/Skyrim.h"


namespace Events
{
	MenuOpenCloseEventHandler* MenuOpenCloseEventHandler::GetSingleton()
	{
		static MenuOpenCloseEventHandler singleton;
		return &singleton;
	}


	auto MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
		-> EventResult
	{
		using Message = RE::UI_MESSAGE_TYPE;

		auto intfcStr = RE::InterfaceStrings::GetSingleton();
		if (a_event->menuName == intfcStr->mainMenu) {
			auto msgQ = RE::UIMessageQueue::GetSingleton();
			msgQ->AddMessage(intfcStr->console, Message::kShow, 0);
		} else if (a_event->menuName == intfcStr->console) {
			auto msgQ = RE::UIMessageQueue::GetSingleton();
			msgQ->AddMessage(intfcStr->console, Message::kHide, 0);
			auto ui = RE::UI::GetSingleton();
			ui->RemoveEventSink(this);
		}

		return EventResult::kContinue;
	}
}
