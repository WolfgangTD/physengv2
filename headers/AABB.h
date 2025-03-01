#include <Vec3.h>

struct AABB{
    Vec3 min;
    Vec3 max;
};


bool AABBvsAABB( AABB a, AABB b )
{
  // Exit with no intersection if found separated along an axis 
  if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
  if(a.max.y < b.min.y || a.min.y > b.max.y) return false;
  // No separating axis found, therefor there is at least one overlapping axis 
  return true;
}