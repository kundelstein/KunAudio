#pragma once

#include "engine_config.h"
#include "mathlib.h"
#include "debug.h"
#include <stdio.h>
#include <list>
#include <vector>
#include <algorithm> // For std::sort...


template<class TMD>
class Cluster
{
	typedef std::list <TMD*> 	ElementList;


public:

	Cluster ()
	{
		is_priority_dirty_ = false;
	}


	~Cluster ()
	{

	}


	void AddElement (TMD* const element, const bool visible)
	{
		if (std::find (total_arr_.begin (), total_arr_.end (), element) == total_arr_.end ())
		{
			total_arr_.push_back (element);

			if (visible)
			{
				AddElementToActiveArr (element);
			}
		}
	}


	void RemoveElement (TMD* const element, const bool visible)
	{
		total_arr_.remove (element);

		if (visible)
		{
			RemoveElementFromActiveArr (element);
		}
	}


	void Update ()
	{
		if (is_priority_dirty_)
		{
			is_priority_dirty_ = false;
			active_arr_.sort (TMD::CompareByPriority);
		}
	}


	void AddElementToActiveArr (TMD* const element)
	{
		DBG_ASSERT (std::find (active_arr_.begin (), active_arr_.end (), element) == active_arr_.end ());

		active_arr_.push_back (element);
		is_priority_dirty_ = true;
	}


	void RemoveElementFromActiveArr (TMD* const element)
	{
		DBG_ASSERT (std::find (active_arr_.begin (), active_arr_.end (), element) != active_arr_.end ());

		active_arr_.remove (element);
	}


	void SetElementPriorityDirtyFlag ()
	{
		is_priority_dirty_ = true;
	}


protected:
public:

	ElementList					active_arr_;


private:

	ElementList					total_arr_;
	bool						is_priority_dirty_;
};

