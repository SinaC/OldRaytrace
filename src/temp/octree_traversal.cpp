CSceneObject* COctree::TraceRay(const CRay& Ray, CVector3& NearestPoint, float& NearestDistance)
{
    // Get the ray origin and direction
    Origin    = Ray.Origin;
    Direction = Ray.Direction;

    // We start at the root node
    COctreeNode* pCurrentNode = m_pRootNode;

    // Locate the starting point leaf node
    while (!pCurrentNode->IsLeaf())
    {
        pCurrentNode = LocatePoint(Child nodes...);
    }

    // Initialize the nearest object and nearest distance
    CSceneObject* pNearestObject = NULL;
    NearestDistance = infinity;

    // Declare variables for intersection testing
    float Distance;
    CVector3 Point;

    // Begin testing for intersections
    do
    {
        // Test all objects of this node for intersection
        for (all objects in this leaf node)
        {
            if (Objectintersected && Distance < NearestDistance)
            {
                pNearestObject   = pCurrentObject;   
                NearestDistance  = Distance;
                NearestPoint     = Point;
            }
        }

        // If we have an intersection, return it
        if (pNearestObject)
            return pNearestObject;

        // If the ray intersects the top plane
        if (Direction.Y > 0)
        {
            // Compute the intersection point
            Point = ...;

            // If the intersection is within our boundaries
            if (We intersect the top plane within our boundaries)
            {
                pCurrentNode = pCurrentNode->TopNeighbor;

                while (!pCurrentNode.IsLeaf())
                {
                    if (Point.X > pCurrentNode->m_Center.Z)
                    {
                        if (Point.Z > pCurrentNode->m_Center.Z)
                        {
                            pCurrentNode = pCurrentNode->m_pNeighbor...
                            break;
                        }
                        else
                        {
                            pCurrentNode = pCurrentNode->m_pNeighbor...
                            break;
                        }
                    }
                    else
                    {
                        ... Same as above 
                    }
                }
            }
        }

        // If the ray intersects the bottom plane
        else if (Direction < 0)
        {
            ... Same as above
        }

        //
        // Same as the two previous blocks for front and back, left and right planes
        //

        // We should never get here with proper rays, but we don't want infinite loops
        return NULL;

    // Repeat as long as we are still in the octree
    } while (pCurrentNode);

    // No match found
    return NULL;
}


