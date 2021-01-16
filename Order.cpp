#include "Order.h"

void Order::BurgerComplete(int n)
{
	burgers_[n].is_complete_ = true;

	// check if all burgers are complete
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		if (burgers_[i].is_complete_ == false)
			return;
	}

	// if we get here all burgers must be complete
	order_complete_ = true;
}
