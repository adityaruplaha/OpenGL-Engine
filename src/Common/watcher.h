#pragma once

#include <atomic>
#include <thread>

enum class WatcherState {
	IDLE,
	ACTIVE,
	STOPPING
};

template <typename T, typename... Args>
class Watcher {
public:
	Watcher(T* parent, void (*watcher_func)(T*, Args...)) : parent(parent), state(WatcherState::IDLE),
	watcher_func(watcher_func) {}

	void start(std::chrono::milliseconds d = std::chrono::milliseconds(1), Args... args) {
		if (state == WatcherState::IDLE)
		{
			watcher_thread = std::thread(&Watcher<T, Args...>::run, this, d, std::forward(args)...);
			state.store(WatcherState::ACTIVE);
		}
	}

	void stop() {
		if (state == WatcherState::ACTIVE)
		{
			state.store(WatcherState::STOPPING);
			watcher_thread.join();
			state.store(WatcherState::IDLE);
		}
	}

	void getState() {
		return std::copy(state.load());
	}

private:
	void run(std::chrono::milliseconds interval, Args... args)
	{
		for (;;)
		{
			watcher_func(parent, std::forward(args)...);
			if (state == WatcherState::STOPPING)
			{
				return;
			}
			std::this_thread::sleep_for(interval);
		}
	}

	std::atomic<WatcherState> state;

	T* parent;
	void (*watcher_func)(T*, Args...);

	std::thread watcher_thread;
};