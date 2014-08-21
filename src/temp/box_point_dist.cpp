bool SquaredDistancePointAABB(const Vector3& p, const Vector3& min, const Vector3& max)
{
    float dsquared = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        if (p[i] > max[i])
            dsquared += (p[i] - max[i]) * (p[i] - max[i]);
        else if (p[i] < min[i])
            dsquared += (min[i] - p[i]) * (min[i] - p[i]);
    }
    
    return dsquared;
} 

