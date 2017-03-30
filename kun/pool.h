#pragma once

#include "debug.h"
#include <stdio.h>
#include <list>
#include <algorithm>


// @TODO: This pool should be used for bigger objects, which most likely won't benefit from DOD.
template<class T>
class Pool
{
public:

	typedef std::list <T*> 	ItemList;

	ItemList			slot_arr_;
	ItemList			used_arr_;
	ItemList			free_arr_;


public:

	Pool ()
	{
		clear ();
	}


	~Pool ()
	{
	}


	// Declare item and extend the base slot.
	void CreateSlot (T* item)
	{
		slot_arr_.push_back (item);
		free_arr_.push_back (item);
	}


	//template<class T>
	T* ReserveSlot ()
	{
		T* const channel = free_arr_.front ();
		used_arr_.push_back (channel);
		free_arr_.pop_front ();

		DBG_ASSERT (slot_arr_.size () == used_arr_.size () + free_arr_.size ());

		return channel;
	}


	void ReleaseSlot (T* item)
	{
		free_arr_.push_back (item);
		used_arr_.remove (item);
		DBG_ASSERT (slot_arr_.size () == used_arr_.size () + free_arr_.size ());
	}


	typename ItemList::const_iterator EraseSlot (typename ItemList::const_iterator iter)
	{
		free_arr_.push_front (*iter);
		return used_arr_.erase (iter);
	}


	void clear ()
	{
		slot_arr_.clear ();
		used_arr_.clear ();
		free_arr_.clear ();
	}


	// Do we have any free slots?
	bool full () const
	{
		return (free_arr_.empty ());
	}


	bool empty () const
	{
		return (used_arr_.empty ());
	}


	typename ItemList::const_iterator begin () const
	{
		return used_arr_.begin ();
	}


	typename ItemList::const_iterator end () const
	{
		return used_arr_.end ();
	}


	size_t size () const
	{
		return used_arr_.size ();
	}


	typename ItemList::const_iterator slot_begin () const
	{
		return slot_arr_.begin ();
	}


	typename ItemList::const_iterator slot_end () const
	{
		return slot_arr_.end ();
	}


	size_t slot_size () const
	{
		return slot_arr_.size ();
	}
};

