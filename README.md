# C++ Simple Quadtree implementation
Offers everything a quadtree has to offer.
### Bonuses
- Closest point finder (requires the point to have the distance function (see Customizations).
- Stocks a value in pair with a point.

# Customizations
1. To customize the max count of points for each subtree
  - check quadtree.hpp
  - Modify QT_NODE_CAPACITY to a value that you like.
2. You can use your own point.
  - Make sure it implements the < operator.
  - Make sure it implements the distance function.
3. Using your own quadtree Shape (Default = Square)
  - Modify AABB struct to your desire
  - Make sure intersectsAABB - containsPoint works as desired
  
# Disclaimer
This version of Quadtree is meant to be simple. It has yet a lot to offer to those who can modify it
accordingly. It was strongly inspired by the pseudo code proposed on [Wikipedia](https://en.wikipedia.org/wiki/Quadtree).
- It wasn't thouroughly tested, therefore many bugs may or may not be present.
- I am not responsible if it crashes (somehow) your pc.
- I am not responsible if you can't read c++ code or don't like my coding style.
- Feel free to propose changes and / or enhancements.
