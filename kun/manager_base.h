#pragma once

#include "mathlib.h"
#include "debug.h"
#include <stdio.h>
#include <list>
#include <algorithm> // For std::sort...


class Engine;


template <typename TMR>
class Manager
{
	friend TMR;


public:

	typedef std::list<TMR*> ManagedList;


public:

	Manager (Engine* const engine_ptr)
		: engine_ptr_ (engine_ptr), is_priority_dirty_ (false)
	{
	}


protected:

	~Manager ()
	{

	}


public:

	void ManagerSortByPriority ()
	{
		if (is_priority_dirty_)
		{
			is_priority_dirty_ = false;
			managed_arr_.sort (TMR::CompareByPriority);
		}
	}


	void ManagerSetPriorityDirtyFlag ()
	{
		is_priority_dirty_ = true;
	}


private:

	void EnableInternal (TMR* const element)
	{
		DBG_ASSERT (std::find (managed_arr_.begin (), managed_arr_.end (), element) == managed_arr_.end ());

		managed_arr_.push_back (element);
		is_priority_dirty_ = true;
	}


	void DisableInternal (TMR* const element)
	{
		managed_arr_.remove (element);
	}


protected:

	Engine*						engine_ptr_;
	ManagedList					managed_arr_;


private:

	bool						is_priority_dirty_;
};


