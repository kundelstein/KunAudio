#pragma once


template <typename CRTP>
class Priorited
{
private:

	int							priority_;


public:

	Priorited ()
		: priority_ (0)
	{
	}


protected:

	~Priorited ()
	{
	}


public:

	void SetPriority (const int priority)
	{
		priority_ = priority;
		static_cast<CRTP*>(this)->PriorityChanged ();
	}


	int GetPriority () const
	{
		return priority_;
	}


	static bool CompareByPriority (const Priorited* const a, const Priorited* const b)
	{
		return (a->priority_ < b->priority_);
	}


	bool operator<(const Priorited& model) const
	{
		return priority_ < model.priority_;
	}
};

