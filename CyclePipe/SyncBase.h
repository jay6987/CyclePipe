#pragma once
#include <shared_mutex>
#include <condition_variable>
#include "Noncopyable.h"

namespace CyclePipe
{
	struct SyncObjectClosedAndEmptySignal {};
	class SyncBase : Noncopyable
	{
	public:
		SyncBase() :closed(false) {}
		virtual ~SyncBase(){};
		void Close();
		bool Closed();
	protected:
		std::shared_mutex mutex;
		std::condition_variable_any condition;
		bool closed;
	};
}
