#pragma once

#include <memory>
#include <atomic>
#include <concepts>

// MPSC queue for lambdas functions

template<typename Func>
class Queue
{
public:
	struct Node
	{
		Func func;
		std::shared_ptr<Node> next = nullptr;
	};

	void Initialize()
	{
		std::shared_ptr<Node> dummy = std::make_shared<Node>();
		m_Head = dummy;
		m_Tail.store(dummy);
	}

	void Push(Func&& data)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>();
		node->func = std::move(data);
		
		std::shared_ptr<Node> prev = m_Tail.exchange(node);
		prev->next = node;
	}

	[[nodiscard]] std::shared_ptr<Node> Pop()
	{
		std::shared_ptr<Node> first = m_Head;
		std::shared_ptr<Node> next = first->next;

		if (next) {
			m_Head = next;
			return next;
		}

		return nullptr;
	}

	void SetProcessingLock(bool value)
	{
		m_IsProcessingLocked = value;
	}

	[[nodiscard]] inline bool IsProcessingLocked()
	{
		return m_IsProcessingLocked;
	}

private:
	std::shared_ptr<Node> m_Head				= nullptr;
	std::atomic<std::shared_ptr<Node>> m_Tail	= nullptr;
	bool m_IsProcessingLocked					= false;		
};