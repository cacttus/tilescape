/*--------------------------------------------

	Class to build off of the object class
	for shops dealing with animations
	etc.

---------------------------------------------*/



class Shop_Object : public Object_Class
{
	long double		Total_Profit;			//Total profit of the shop
	int				Total_Customers;		//Total customers

	int				Avg_Cust_Hour;			//Average customers per hour

public:

	void Update();			//The core update function
	void Draw();			//The core drawing function


};