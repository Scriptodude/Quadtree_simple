#ifndef DEF_QUAD_TREE_HPP
#define DEF_QUAD_TREE_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

struct point
{
	inline point() = default;
	inline point(double xx, double yy) : x(xx), y(yy) {}
	inline double distanceSquare(point const &o) { return (x-o.x)*(x-o.x) + (y-o.y)*(y-o.y);}
	inline double distance(point const &o) { return std::sqrt(distanceSquare(o)); }
	double x;
	double y;

	inline bool operator <(point const &o) const { return (y * 1000 + x) < (o.y * 1000 + x); }
};


template<class T>
class Quadtree
{
	typedef std::pair<point, T> values;
	typedef typename std::vector<values>::iterator values_it;
	typedef typename std::vector<values>::const_iterator values_const_it;
	static const size_t QT_NODE_CAPACITY = 75;
public:
	typedef std::list<values> points;
	typedef typename points::iterator points_it;
	

	struct AABB
	{
		inline AABB() = default;
		inline AABB(point const &c, double half) : center(c), half_dim(half) {}
		inline AABB(double x, double y, double size) : center(x, y), half_dim(size/2) {}
		bool containsPoint(point const &p) const;
		bool intersectAABB(AABB const &o) const;
		point center;
		double half_dim;
	};

	inline Quadtree() = default;
	inline Quadtree(AABB bounds)
		: 	m_points(),
	 		m_boundary(bounds),
	 		north_w(NULL),
	 		north_e(NULL),
	 		south_w(NULL),
	 		south_e(NULL)
	 		{}

	~Quadtree();

	bool insert(point const &p, T const &val);
	std::list<values> queryRange(AABB const &range) const;
	T findNearest(AABB const &range) const;

private:
	std::vector<values> m_points;
	AABB m_boundary;
	Quadtree *north_w, *north_e, *south_w, *south_e;

	void _subdivide();
};
#endif

#include <iostream>
#include <limits>

using namespace std;

/* Substructures definitions */
template<class T>
bool Quadtree<T>::AABB::containsPoint(point const &p) const 
{
	double minx, maxx, miny, maxy;
	minx = center.x - half_dim;
	maxx = center.x + half_dim;
	miny = center.y - half_dim;
	maxy = center.y + half_dim;

	return 	minx <= p.x && p.x <= maxx &&
			miny <= p.y && p.y <= maxy;
}

template<class T>
bool Quadtree<T>::AABB::intersectAABB(const Quadtree::AABB &o) const 
{
	double minx, maxx, miny, maxy;
	minx = center.x - half_dim;
	maxx = center.x + half_dim;
	miny = center.y - half_dim;
	maxy = center.y + half_dim;

	double minx1, maxx1, miny1, maxy1;
	minx1 = o.center.x - o.half_dim;
	maxx1 = o.center.x + o.half_dim;
	miny1 = o.center.y - o.half_dim;
	maxy1 = o.center.y + o.half_dim;

	return (minx < maxx1 && maxx > minx1 &&
			miny < maxy1 && maxy > miny1);
}

/************************/
/* Quadtree definitions */
/************************/

/************************/
/****Public functions ***/
/************************/


template<class T>
Quadtree<T>::~Quadtree()
{
	delete north_w;
	delete north_e;
	delete south_w;
	delete south_e;
}

template<class T>
const size_t Quadtree<T>::QT_NODE_CAPACITY;

template<class T>
bool Quadtree<T>::insert(point const &p, T const &val)
{
	if(!m_boundary.containsPoint(p))
		return false;

	if(m_points.size() < Quadtree::QT_NODE_CAPACITY)
	{
		m_points.push_back(make_pair(p, val));
		return true;
	}

	if(north_w == NULL)
		_subdivide();

	if(north_w->insert(p, val)) return true;
	if(north_e->insert(p, val)) return true;
	if(south_w->insert(p, val)) return true;
	if(south_e->insert(p, val)) return true;

	return false;
}

template<class T>
list<typename Quadtree<T>::values> Quadtree<T>::queryRange(AABB const &range) const
{
	list<values> points_in_range;

	if(!m_boundary.intersectAABB(range))
		return points_in_range;

	values_const_it it = m_points.cbegin();
	for(;it != m_points.cend(); ++it)
	{
		if(range.containsPoint(it->first))
			points_in_range.push_back(make_pair(it->first, it->second));
	}

	if(north_w == NULL)
		return points_in_range;

	points_in_range.merge(north_w->queryRange(range));
	points_in_range.merge(north_e->queryRange(range));
	points_in_range.merge(south_w->queryRange(range));
	points_in_range.merge(south_e->queryRange(range));

	return points_in_range;
}

template<class T>
T Quadtree<T>::findNearest(typename Quadtree<T>::AABB const &range) const
{
	points p = queryRange(range);
	points_it it = p.begin();
	double dist_min = numeric_limits<double>::infinity();
	T min = T();

	for(; it != p.end(); ++it)
	{
		double d = it->first.distance(range.center);
		if(d < dist_min)
		{
			min = it->second;
			dist_min = d;
		}
	}

	return min;
}

/************************/
/****Private functions **/
/************************/

template<class T>
void Quadtree<T>::_subdivide()
{
	point nwp, nep, swp, sep;
	double 	hd2 = m_boundary.half_dim/2;
	point 	cent = m_boundary.center;

	nwp = point( cent.x - hd2, cent.y - hd2 );
	nep = point( cent.x + hd2, cent.y - hd2 );
	swp = point( cent.x - hd2, cent.y + hd2 );
	sep = point( cent.x + hd2, cent.y + hd2 );

	AABB 	nwb(nwp, hd2),
	 		neb(nep, hd2),
	 		swb(swp, hd2),
	 		seb(sep, hd2);

	north_w = new Quadtree(nwb);
	north_e = new Quadtree(neb);
	south_w = new Quadtree(swb);
	south_e = new Quadtree(seb);
}