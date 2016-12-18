#include <iostream>
#include "quadtree.hpp"

int main()
{
	Quadtree<int>::AABB b(point(50, 50), 50);
	Quadtree<int> q = Quadtree<int>(b);
	
	for(int i=0; i<1000000; i++)
	{
		if(i % 15 != 0)
			q.insert(point(i % 75, i % 75), i);
	}	

	Quadtree<int>::AABB r(point(5, 5), 3);

	Quadtree<int>::points vals = q.queryRange(r);
	Quadtree<int>::points_it it = vals.begin();

	cout << "Nearest : " << q.findNearest(Quadtree<int>::AABB(15, 15, 15)) << endl;

	return 0;
}