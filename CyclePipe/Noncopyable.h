#pragma once

namespace CyclePipe
{
	class Noncopyable
	{
	protected:
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable(Noncopyable&&) = delete;
		Noncopyable& operator = (const Noncopyable&) = delete;
		Noncopyable& operator = (Noncopyable&&) = delete;

		Noncopyable() = default;
		~Noncopyable() = default;
	};
}
