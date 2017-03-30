#pragma once

#include "mathlib.h"
#include "debug.h"


template <typename TMD, typename CRTP>
class Managed
{
	friend TMD;
	friend CRTP;


public:

	Managed (const int priority)
		: is_managed_ (false), is_active_ (true), priority_ (priority)
	{
	}


protected:

	~Managed ()
	{
	}


public:

	void Enable ()
	{
		if (is_active_)
			return;

		is_active_ = true;

		if (is_managed_)
		{
			static_cast<CRTP*>(this)->EnableInternal ();
		}
	}


	void Disable ()
	{
		if (!is_active_)
			return;

		if (is_managed_)
		{
			static_cast<CRTP*>(this)->DisableInternal ();
		}

		is_active_ = false;
	}


	void SetActive (const bool active)
	{
		if (is_active_ == active)
			return;

		if (is_managed_)
		{
			if (is_active_)
			{
				static_cast<CRTP*>(this)->DisableInternal ();
				is_active_ = false;
			}
			else
			{
				is_active_ = true;
				static_cast<CRTP*>(this)->EnableInternal ();
			}
		}
		else
		{
			is_active_ = active;
		}
	}


	bool IsActive () const
	{
		return is_active_;
	}


	void AddToManaged ()
	{
		if (is_managed_)
			return;

		is_managed_ = true;

		if (is_active_)
		{
			static_cast<CRTP*>(this)->EnableInternal ();
		}
	}


	void RemoveFromManaged ()
	{
		if (!is_managed_)
			return;

		if (is_active_)
		{
			static_cast<CRTP*>(this)->DisableInternal ();
		}

		is_managed_ = false;
	}


	void SetPriority (const int priority)
	{
		priority_ = priority;

		if (is_managed_)
		{
			manager_->ManagerSetPriorityDirtyFlag ();
		}
	}


	static bool CompareByPriority (const Managed* const a, const Managed* const b)
	{
		return (a->priority_ < b->priority_);
	}


	bool operator<(const Managed& layer) const
	{
		return priority_ < layer.priority_;
	}
	

private:

	bool						is_active_;
	bool						is_managed_;
	int							priority_;


protected:

	static	TMD*				manager_;				// Should be used only by Engine.
};


template <typename TMD, typename CRTP>
TMD* Managed<TMD, CRTP>::manager_;

