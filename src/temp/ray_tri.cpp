bool intersectTriangle( const TPoint3 &orig, const TVector3 &dir,
			const TPoint3 &vert0, const TPoint3 &vert1, const TPoint3 &vert2,
			float &t, float &u, float &v ) {
  // find vectors for 2 edges sharing vert0
  TVector3 edge1 = vert1 - vert0;
  TVector3 edge2 = vert2 - vert0;
  // begin calculating determinany - also used to calculate U parameter
  TVector3 pvec = crossProduct( dir, edge2 );
  // if determinant is near zero, ray lies in plane of triangle
  float det = edge1 | pvec;
#ifdef TEST_CULL // define TEST_CULL is culling is desired
  if ( det < EPS )
    return false;
  // calculate distance from vert0 to ray origin
  TVector3 tvec = orig - vert0;
  // calculate U paramter and test bounds
  u = tvec | pvec;
  if ( u < 0.0f || u > det )
    return false;
  // prepare to test V parameter
  TVector3 qvec = crossProduct( tvec, edge1 );
  // calculate V parameter and test bounds
  v = dir | qvec;
  if ( v < 0.0f || u + v > det )
    return false;
  // calculate t, scale parameters, ray intersects triangle
  t = edge2 | qvec;
  float invDet = 1.0f / det;
  t *= invDet;
  u *= invDet;
  v *= invDet;
#else // the non-culling branch
  if ( det > -EPS && det < EPS )
    return false;
  float invDet = 1.0f / det;
  // calculate distance from vert0 to ray origin
  TVector3 tvec = orig - vert0;
  // calculate U parameter and test bounds
  u = ( tvec | pvec ) * invDet;
  if ( u < 0.0f || u > 1.0f )
    return false;
  // prepare to test V parameter
  TVector3 qvec = crossProduct( tvec, edge1 );
  // calculate V parameter and test bounds
  v = ( dir | qvec ) * invDet;
  if ( v < 0.0f || v > 1.0f )
    return false;
  // calculate t, ray intersects triangle
  t = ( edge2 | qvec ) * invDet;
#endif
  return true;
}
