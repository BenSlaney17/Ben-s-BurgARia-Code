#pragma once
#include "FoodTypes.h"
#include <vector>

enum OrderTime
{
	Short, Medium, Long, Last
};

struct BurgerComponents
{
	int num_burgers_;
	int num_onions_;
	int num_tomatoes_;
	bool is_complete_;
};
struct Order
{
	Order()
	{
		burgers_ = std::vector<BurgerComponents>();
		order_complete_ = false;
	}

	void BurgerComplete(int n);

	std::vector<BurgerComponents> burgers_;
	OrderTime order_time_type_;
	float time_of_order_;
	bool order_complete_;
};

