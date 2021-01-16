#include "OrderGenerator.h"
#include <math.h>
Order OrderGenerator::GenerateOrder(float cur_time)
{
	Order order = Order();
	// choose random number of burgers
	int numOfBurgers = 2;// rand() % 2 + 1;
	 for (size_t i = 0; i < numOfBurgers; i++)
	 {
		 order.burgers_.push_back(GenerateBurger());
	 }

	// Used for generating random order time https://stackoverflow.com/questions/2999012/generating-random-enums 
	order.order_time_type_ = static_cast<OrderTime>(rand() % Last);
	
	order.time_of_order_ = cur_time;
	
	return order;
}

BurgerComponents OrderGenerator::GenerateBurger()
{
	BurgerComponents comp;
	comp.num_burgers_ = rand() % 2 + 1;
	comp.num_onions_ = rand() % 2 + 1;
	comp.num_tomatoes_ = rand() % 2 + 1;
	comp.is_complete_ = false;
	return comp;
}
