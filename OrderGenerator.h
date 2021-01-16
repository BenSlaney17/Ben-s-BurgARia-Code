#pragma once
#include "Order.h"
static class OrderGenerator
{
public: 
	static Order GenerateOrder(float cur_time);

private:
	static BurgerComponents GenerateBurger();
};

