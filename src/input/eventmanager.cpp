#include "eventmanager.h"

EventManager *EventManager::default_manager;

EventManager::EventManager() {}

EventManager::~EventManager() {}

void EventManager::register_event(Event *e) { events.push_back(e); }

void EventManager::clear_event(Event *e) { events.clear(e); }

void EventManager::clean() { events.delete_all(); }

void EventManager::init() { default_manager = new EventManager; }

EventManager &EventManager::get_manager() { return *default_manager; }
