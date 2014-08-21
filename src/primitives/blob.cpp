#include <stdio.h>

#include "blob.h"
#include "../intersection.h"
#include "../ray.h"
#include "../math/solver.h"

const float insideTolerance = 1.0e-6f;

const TTagList PrimitiveBlob::primitiveBlobTags [] = {
  { "threshold", TAG_primitiveBlob_threshold },
  { "blobs", TAG_primitiveBlob_blobs },
  { NULL, -1 }
};

///////////////
// Blob Basic
//
const TTagList BlobSubObject::blobTags [] = {
  { "origin", TAG_blob_origin },
  { "strength", TAG_blob_strength },
  { NULL, -1 }
};

const TTagList BlobSubObject::blobSubClassTags [] = {
  { "sphere", BLOB_SPHERE },
  { "cylinder", BLOB_CYLINDER },
  { "base hemisphere (generated)", BLOB_BASE_HEMISPHERE },
  { "apex hemisphere (generated)", BLOB_APEX_HEMISPHERE },
  {  NULL, -1 }
};

BlobSubObject::BlobSubObject() {
  THandle::THandle(); handleKind = HANDLE_BLOB;
  blobKind = BLOB_NOTHING;
}

void BlobSubObject::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", blobName() );
  origin.print("origin: ");
  int i;
  for ( i = 0; i < 3; i++ )
    printf("coeff[%d]: %5.5f\n", i, c[i] );
  for ( i = 0; i < 5; i++ )
    printf("final coeff[%d]: %5.5f\n", i, f[i] );
}

char *BlobSubObject::blobName() const {
  for ( int i = 0; blobSubClassTags[i].tagName != NULL; i++ )
    if ( blobSubClassTags[i].tagId == blobKind )
      return blobSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", blobKind );
  return buf;
}

///////////////
// BlobSphere
//
const TTagList BlobSphere::blobSphereTags [] = {
  { "radius2", TAG_blobsphere_radius2 },
  { NULL, -1 }
};

BlobSphere::BlobSphere()
: BlobSubObject() {
	blobKind = BLOB_SPHERE;
}

void BlobSphere::setup( vector<BlobSubObject*> &blobs ) {
  float coeff = c[2];

  c[0] = coeff / ( radius2 * radius2 );
  c[1] = -( 2.0f * coeff ) / radius2;
  c[2] = coeff;
}

void BlobSphere::normalAt( TVector3 &normal, const TPoint3 &p ) const {
  TVector3 V1 = p - origin;
  float dist = V1.magnitudeSquared();
  if ( dist <= radius2 ) {
    float val = -2.0f * c[0] * dist - c[1];
    normal = normal + ( val * V1 );
  }
}

float BlobSphere::calculateFieldValue( const TPoint3 &p ) const {
  float density = 0.0f;
  TVector3 V1 = p - origin;
  float r2 = V1.magnitudeSquared();
  if ( r2 <= radius2 )
    density = r2 * ( r2 * c[0] + c[1] ) + c[2];
  return density;
}

void BlobSphere::computeCoeff( const TPoint3 &p, const TVector3 &d ) {
  TVector3 V1 = p - origin;
  float t0 = V1.magnitudeSquared();
  float t1 = V1 | d;
  float c0 = c[0];
  float c1 = c[1];
  float c2 = c[2];

  f[0] = c0;
  f[1] = 4.0f * c0 * t1;
  f[2] = 2.0f * c0 * ( 2.0f * t1 * t1 + t0 ) + c1;
  f[3] = 2.0f * t1 * ( 2.0f * c0 * t0 + c1 );
  f[4] = t0 * ( c0 * t0 + c1 ) + c2;
}

bool BlobSphere::intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const {
  tmin = INFINITE;
  tmax = -INFINITE;

  TVector3 V1 = ray.origin - origin;
  float b = V1 | ray.direction;
  float c = V1.magnitudeSquared() + radius2;

  float d = b * b - c;

  if ( d < EPS )
    return false;

  d = sqrt(d);

  tmax = - b + d;  if ( tmax < mindist ) { tmax = 0.0f; }
  tmin = - b - d;  if ( tmin < mindist ) { tmin = 0.0f; }

  if ( tmax == tmin )
    return false;
  else
    if ( tmax < tmin) {
      d = tmin;  tmin = tmax;  tmax = d;
    }

  return true;
}

void BlobSphere::print( const char *msg ) const {
  BlobSubObject::print( msg );
  printf("radius2: %5.5f\n", radius2 );
}

//////////////////
// BlobCylinder
//
const TTagList BlobCylinder::blobCylinderTags [] = {
  { "radius2", TAG_blobcylinder_radius2 },
  { "length", TAG_blobcylinder_len },
  { NULL, -1 }
};

BlobCylinder::BlobCylinder()
: BlobSubObject() {
   blobKind = BLOB_CYLINDER;
}

void BlobCylinder::setup( vector<BlobSubObject*> &blobs ) {
  // c[2] is blob strength
  float coeff = c[2];
  // create cylindrical component
  c[0] = coeff / ( radius2 * radius2 );
  c[1] = -( 2.0f * coeff ) / radius2;
  c[2] = coeff;
  // create hemispherical at base
  {
    BlobBaseHemisphere *hemi = new BlobBaseHemisphere;
    hemi->origin = origin;
    hemi->c[0] = coeff / ( radius2 * radius2 );
    hemi->c[1] = -( 2.0f * coeff ) / radius2;
    hemi->c[2] = coeff;
    hemi->radius2 = radius2;
    blobs.push_back( hemi );
  }
  // create hemispherical at apex
  {
    BlobApexHemisphere *hemi = new BlobApexHemisphere;
    hemi->origin = origin + TVector3( 0, 0, len );
    hemi->c[0] = coeff / ( radius2 * radius2 );
    hemi->c[1] = -( 2.0f * coeff ) / radius2;
    hemi->c[2] = coeff;
    hemi->len = len;
    hemi->radius2 = radius2;
    blobs.push_back( hemi );
  }
}

void BlobCylinder::normalAt( TVector3 &normal, const TPoint3 &p ) const {
  float dist;
  if ( p[__Z] >= 0.0f
       && p[__Z] <= len
       && ( dist = SQR(p[__X]) + SQR(p[__Y]) ) <= radius2 ) {
    float val = -2.0f * c[0] * dist - c[1];
    normal = normal + ( val * TVector3( p[__X], p[__Y], 0.0f ) );
  }
}

float BlobCylinder::calculateFieldValue( const TPoint3 &p ) const {
  float density = 0.0f;
  if ( p[__Z] >= 0.0f
       && p[__Z] <= len ) {
    float r2 = SQR(p[__X]) + SQR(p[__Y]);
    if ( r2 <= radius2 )
      density = r2 * ( r2 * c[0] + c[1] ) + c[2];
  }
  return density;
}

void BlobCylinder::computeCoeff( const TPoint3 &p, const TVector3 &d ) {
  float t0 = p[__X] * p[__X] + p[__Y] * p[__Y];
  float t1 = p[__X] * d[__X] + p[__Y] * d[__Y];
  float t2 = d[__X] * d[__X] + d[__Y] * d[__Y];
  
  float c0 = c[0];
  float c1 = c[1];
  float c2 = c[2];
        
  f[0] = c0 * t2 * t2;
  f[1] = 4.0f * c0 * t1 * t2;
  f[2] = 2.0f * c0 * ( 2.0f * t1 * t1 + t0 * t2 ) + c1 * t2;
  f[3] = 2.0f * t1 * ( 2.0f * c0 * t0 + c1 );
  f[4] = t0 * ( c0 * t0 + c1 ) + c2;
}

bool BlobCylinder::intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const {
  tmin = INFINITE;
  tmax = -INFINITE;
  float a, b, c, d, t, u, v, w;

  // Intersect ray with cylinder.

  a = ray.direction[__X] * ray.direction[__X] + ray.direction[__Y] * ray.direction[__Y];

  if ( a > EPS ) {
    b = ray.origin[__X] * ray.direction[__X] + ray.origin[__Y] * ray.direction[__Y];
    c = ray.origin[__X] * ray.origin[__X] + ray.origin[__Y] * ray.origin[__Y] - radius2;

    d = b * b - a * c;

    if ( d > EPS ) {
      d = sqrt(d);
      
      t = ( - b + d) / a;

      w = ray.origin[__Z] + t * ray.direction[__Z];

      if ( (w >= 0.0f) && (w <= len) ) {
        if ( t < tmin ) tmin = t;
        if ( t > tmax ) tmax = t;
      }

      t = ( - b - d) / a;

      w = ray.origin[__Z] + t * ray.direction[__Z];

      if ( (w >= 0.0f) && (w <= len) ) {
        if ( t < tmin ) tmin = t;
        if ( t > tmax ) tmax = t;
      }
    }
  }

  // Intersect base/cap plane.
  if ( fabsf(ray.direction[__Z]) > EPS ) {
    // Intersect base plane.
    t = - ray.origin[__Z] / ray.direction[__Z];

    u = ray.origin[__X] + t * ray.direction[__X];
    v = ray.origin[__Y] + t * ray.direction[__Y];

    if ( (u * u + v * v) <= radius2 ) {
      if ( t < tmin ) tmin = t;
      if ( t > tmax ) tmax = t;
    }

    // Intersect cap plane.
    t = ( len - ray.origin[__Z] ) / ray.direction[__Z];

    u = ray.origin[__X] + t * ray.direction[__X];
    v = ray.origin[__Y] + t * ray.direction[__Y];

    if ( (u * u + v * v) <= radius2 ) {
      if ( t < tmin ) tmin = t;
      if ( t > tmax ) tmax = t;
    }
  }

  // Check if the intersections are valid.

  if ( tmin < mindist ) tmin = 0.0f;
  if ( tmax < mindist ) tmax = 0.0f;

  if ( tmin >= tmax )
    return false;

  return true;
}

void BlobCylinder::print( const char *msg ) const {
  BlobSubObject::print( msg );
  printf("radius2: %5.5f\n", radius2 );
  printf("length: %5.5f\n", len );
}

///////////////////////
// BlobBaseHemisphere
//

BlobBaseHemisphere::BlobBaseHemisphere()
: BlobSubObject() { 
	blobKind = BLOB_BASE_HEMISPHERE;
}

void BlobBaseHemisphere::setup( vector<BlobSubObject*> &blobs ) {
}

void BlobBaseHemisphere::normalAt( TVector3 &normal, const TPoint3 &p ) const {
  if ( p[__Z] <= 0.0f ) {
    float dist = p.magnitudeSquared();
    if ( dist <= radius2 ) {
      float val = -2.0f * c[0] * dist - c[1];
      normal = normal + ( val * p );
    }
  }
}

float BlobBaseHemisphere::calculateFieldValue( const TPoint3 &p ) const {
  float density = 0.0f;
  if ( p[__Z] <= 0.0f ) {
    float r2 = p.magnitudeSquared();
    if ( r2 <= radius2 )
      density = r2 * ( r2 * c[0] + c[1] ) + c[2];
  }
  return density;
}

void BlobBaseHemisphere::computeCoeff( const TPoint3 &p, const TVector3 &d ) {
  float t0 = p.magnitudeSquared();
  float t1 = p | d;
  float t2 = d.magnitudeSquared();
  float c0 = c[0];
  float c1 = c[1];
  float c2 = c[2];

  f[0] = c0 * t2 * t2;
  f[1] = 4.0f * c0 * t1 * t2;
  f[2] = 2.0f * c0 * ( 2.0f * t1 * t1 + t0 * t2 ) + c1 * t2;
  f[3] = 2.0f * t1 * ( 2.0f * c0 * t0 + c1 );
  f[4] = t0 * ( c0 * t0 + c1 ) + c2;
}

bool BlobBaseHemisphere::intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const {
  tmin = INFINITE;
  tmax = -INFINITE;
  float b, t, d, z1, z2;

  b = ray.origin | ray.direction;
  t = ray.origin.magnitudeSquared();

  d = b * b - t + radius2;

  if ( d < EPS )
    return false;

  d = sqrt(d);

  tmax = - b + d;
  tmin = - b - d;

  if ( tmax < tmin ) {
    d = tmin;  tmin = tmax;  tmax = d;
  }

  // Cut intersection at the plane.
  
  z1 = ray.origin[__Z] + tmin * ray.direction[__Z];
  z2 = ray.origin[__Z] + tmax * ray.direction[__Z];

  // If both points are inside --> no intersection
  
  if ( (z1 >= 0.0f) && (z2 >= 0.0f) )
    return false;

  // If both points are outside --> intersections found

  if ( (z1 < 0.0f) && (z2 < 0.0f) )
    return true;

  // Determine intersection with plane.
  
  t = - ray.origin[__Z] / ray.direction[__Z];

  if ( z1 >= 0.0f )
    // Ray is crossing the plane from inside to outside.
    tmin = (t < mindist) ? 0.0 : t;
  else
    // Ray is crossing the plane from outside to inside.
    tmax = (t < mindist) ? 0.0 : t;

  return true;
}

void BlobBaseHemisphere::print( const char *msg ) const {
  BlobSubObject::print( msg );
  printf("radius2: %5.5f\n", radius2 );
}

///////////////////////
// BlobApexHemisphere
//

BlobApexHemisphere::BlobApexHemisphere()
:BlobSubObject() {
   blobKind = BLOB_APEX_HEMISPHERE;
}

void BlobApexHemisphere::setup( vector<BlobSubObject*> &blobs ) {
}

void BlobApexHemisphere::normalAt( TVector3 &normal, const TPoint3 &p ) const {
  float z = p[__Z] - len;
  if ( z >= 0.0f ) {
    float dist = SQR(p[__X]) + SQR(p[__Y]) + SQR(z);
    if ( dist <= radius2 ) {
      float val = -2.0f * c[0] * dist - c[1];
      normal = normal + ( val * TVector3( p[__X], p[__Y], z ) );
    }
  }
}

float BlobApexHemisphere::calculateFieldValue( const TPoint3 &p ) const {
  float density = 0.0f;
  float z = p[__Z] - len;
  if ( z >= 0.0f ) {
    float r2 = SQR(p[__X]) + SQR(p[__Y]) + z*z;
    if ( r2 <= radius2 )
      density = r2 * ( r2 * c[0] + c[1] ) + c[2];
  }
  return density;
}

void BlobApexHemisphere::computeCoeff( const TPoint3 &p, const TVector3 &d ) {
  float z = p[__Z] - len;
  float t0 = SQR(p[__X]) + SQR(p[__Y]) + SQR(z);
  float t1 = p[__X] * d[__X] + p[__Y] * d[__Y] + z * d[__Z];
  float t2 = d.magnitudeSquared();
  float c0 = c[0];
  float c1 = c[1];
  float c2 = c[2];

  f[0] = c0 * t2 * t2;
  f[1] = 4.0f * c0 * t1 * t2;
  f[2] = 2.0f * c0 * ( 2.0f * t1 * t1 + t0 * t2 ) + c1 * t2;
  f[3] = 2.0f * t1 * ( 2.0f * c0 * t0 + c1 );
  f[4] = t0 * ( c0 * t0 + c1 ) + c2;
}

bool BlobApexHemisphere::intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const {
  tmin = INFINITE;
  tmax = -INFINITE;

  float b, t, d, z1, z2;
  float z = ray.origin[__Z]-len;

  // PP * DD
  b = ray.origin[__X] * ray.direction[__X] + ray.origin[__Y] * ray.direction[__Y] + z * ray.direction[__Z];
  t = SQR(ray.origin[__X]) + SQR(ray.origin[__Y]) + SQR(z);
  
  d = b * b - t + radius2;

  if ( d < EPS )
    return false;

  d = sqrt(d);
  
  tmax = - b + d;
  tmin = - b - d;

  if ( tmax < tmin ) {
    d = tmin;  tmin = tmax;  tmax = d;
  }
  
  // Cut intersection at the plane.
  
  z1 = z + tmin * ray.direction[__Z];
  z2 = z + tmax * ray.direction[__Z];

  // If both points are inside --> no intersection

  if ( (z1 <= 0.0f) && (z2 <= 0.0f) )
    return false;
    
  // If both points are outside --> intersections found
  
  if ( (z1 > 0.0f) && (z2 > 0.0f) )
    return true;

  // Determine intersection with plane.
  
  t = - z / ray.direction[__Z];

  if ( z1 <= 0.0f )
    // Ray is crossing the plane from inside to outside.
      tmin = (t < mindist) ? 0.0 : t;
  else
    // Ray is crossing the plane from outside to inside.
    tmax = (t < mindist) ? 0.0 : t;

  return true;
}

void BlobApexHemisphere::print( const char *msg ) const {
  BlobSubObject::print( msg );
  printf("radius2: %5.5f\n", radius2 );
  printf("length: %5.5f", len );
}

//-------------
//---- Blob set

PrimitiveBlob::PrimitiveBlob() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_BLOB;
  //  numBlobs = 0;
  intervals = NULL;
  intervalCount = 0;
}

PrimitiveBlob::~PrimitiveBlob() {
  blobs.clear();
  if ( intervals != NULL )
    delete [] intervals;
  intervals = NULL;
}

void PrimitiveBlob::setup() {
  vector<BlobSubObject *>::const_iterator endLoop = blobs.end();
  for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	iter != endLoop;
	iter++ )
    (*iter)->setup( blobs );
//   vector<BlobSubObject *>::const_iterator iterNext;
//   for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
// 	iter != blobs.end();
// 	iter = iterNext ) {
//     iterNext = iter++;
//     (*iter)->setup( blobs );
//   }
  if ( intervals == NULL )
    intervals = new TInterval [blobs.size()*2];
  intervalCount = 0;
}

void PrimitiveBlob::precalcFrame( const TWorld &world ) {
  intervalCount = 0;
}

bool PrimitiveBlob::intersectionRay( const TRay &ray,
				     TIntersection &inter ) const {

  TIntersectionStack IStack;
  IStack.create( 16 );
  if ( !PrimitiveBlob::allIntersectionRay( ray, IStack ) )
    return false;
  bool found = false;
  inter.distance = INFINITE;
  for ( int i = 0; i < IStack.index; i++ )
    if ( IStack.stack[i].distance < inter.distance
	 && IStack.stack[i].distance > 0.0f ) {
      found = true;
      inter.distance = IStack.stack[i].distance;
    }
  return found;

//--FIXME--  int i, j;
//--FIXME--
//--FIXME--  // Get the intervals along the ray where each component has an effect.
//--FIXME--  int cnt;
//--FIXME--  ((PrimitiveBlob*)this)->intervalCount = 0;
//--FIXME--  if ( ( cnt = determineInfluence( ray, 1.0e-2f ) ) == 0 )
//--FIXME--    // Ray doesn't hit any of the component elements.
//--FIXME--    return false;
//--FIXME--
//--FIXME--  // To avoid numerical problems we start at the first interval.
//--FIXME--  float startDist;
//--FIXME--  if ( ( startDist = intervals[0].bound ) < 0.001f )
//--FIXME--    startDist = 0.0f;
//--FIXME--
//--FIXME--  for ( i = 0; i < cnt; i++ )
//--FIXME--    intervals[i].bound -= startDist;
//--FIXME--
//--FIXME--  // Get the new starting point.
//--FIXME--  TPoint3 P = ray.origin + ( startDist * ray.direction );
//--FIXME--
//--FIXME--  // Clear out the coefficients.
//--FIXME--  double coeffs[5];
//--FIXME--  coeffs[0] =
//--FIXME--  coeffs[1] =
//--FIXME--  coeffs[2] =
//--FIXME--  coeffs[3] = 0.0;
//--FIXME--  coeffs[4] = - (double)threshold;
//--FIXME--  
//--FIXME--  // Step through the list of intersection points, adding the
//--FIXME--  // influence of each component as it appears. 
//--FIXME--  int inFlag;
//--FIXME--  bool intersectionFound = false;
//--FIXME--  for ( i = inFlag = 0; i < cnt; i++) {
//--FIXME--    if ( intervals[i].type == ENTERING ) {
//--FIXME--      // Something is just starting to influence the ray, so calculate
//--FIXME--      // its coefficients and add them into the pot. 
//--FIXME--      inFlag++;
//--FIXME--
//--FIXME--      intervals[i].element->computeCoeff( P, ray.direction );
//--FIXME--      
//--FIXME--      for ( j = 0; j < 5; j++ )
//--FIXME--        coeffs[j] += (double)intervals[i].element->f[j];
//--FIXME--    }
//--FIXME--    else {
//--FIXME--      // We are losing the influence of a component -->
//--FIXME--      // subtract off its coefficients. 
//--FIXME--      
//--FIXME--      for ( j = 0; j < 5; j++ )
//--FIXME--        coeffs[j] -= (double)intervals[i].element->f[j];
//--FIXME--      
//--FIXME--      // If no components are currently affecting the ray ---> skip ahead.
//--FIXME--      if ( --inFlag == 0 )
//--FIXME--        continue;
//--FIXME--    }
//--FIXME--
//--FIXME--    // If the following intersection lies close to the current intersection
//--FIXME--    // then first add/subtract next region before testing. [DB 7/94] 
//--FIXME--    
//--FIXME--    if ( ( i + 1 < cnt )
//--FIXME--	 && ( fabsf( intervals[i].bound - intervals[i + 1].bound ) < EPS ) )
//--FIXME--      continue;
//--FIXME--
//--FIXME--    // Transform polynomial in a way that the interval boundaries are moved
//--FIXME--    // to 0 and 1, i. e. the roots of interest are between 0 and 1.
//--FIXME--    float l = intervals[i].bound;
//--FIXME--    float w = intervals[i+1].bound - l;
//--FIXME--    float newcoeffs[5];
//--FIXME--    newcoeffs[0] = coeffs[0] * w * w * w * w;
//--FIXME--    newcoeffs[1] = ( coeffs[1] + 4.0f * coeffs[0] * l ) * w * w * w;
//--FIXME--    newcoeffs[2] = ( 3.0f * l * ( 2.0f * coeffs[0] * l + coeffs[1] ) + coeffs[2] ) * w * w;
//--FIXME--    newcoeffs[3] = ( 2.0f * l * ( 2.0f * l * ( coeffs[0] * l + 0.75f * coeffs[1] ) + coeffs[2] ) + coeffs[3]) * w;
//--FIXME--    newcoeffs[4] = l * ( l * ( l * ( coeffs[0] * l + coeffs[1] ) + coeffs[2] ) + coeffs[3] ) + coeffs[4];
//--FIXME--
//--FIXME--    // Calculate coefficients of corresponding bezier curve.
//--FIXME--    float dk[5];
//--FIXME--    dk[0] = newcoeffs[4];
//--FIXME--    dk[1] = newcoeffs[4] + 0.25f * newcoeffs[3];
//--FIXME--    dk[2] = newcoeffs[4] + 0.50f * ( newcoeffs[3] + newcoeffs[2] / 12.0f );
//--FIXME--    dk[3] = newcoeffs[4] + 0.50f * ( 0.375f * newcoeffs[3] + newcoeffs[2] + 0.125f * newcoeffs[1] );
//--FIXME--    dk[4] = newcoeffs[4] + newcoeffs[3] + newcoeffs[2] + newcoeffs[1] + newcoeffs[0];
//--FIXME--
//--FIXME--    // Skip this interval if the ray doesn't intersect the convex hull of the
//--FIXME--    // bezier curve, because no valid intersection will be found.
//--FIXME--
//--FIXME--    if ( ( dk[0] >= 0.0f && dk[1] >= 0.0f && dk[2] >= 0.0f && dk[3] >= 0.0f && dk[4] >= 0.0f ) ||
//--FIXME--	 ( dk[0] <= 0.0f && dk[1] <= 0.0f && dk[2] <= 0.0f && dk[3] <= 0.0f && dk[4] <= 0.0f ) )
//--FIXME--      continue;
//--FIXME--
//--FIXME--    // Now we could do bezier clipping to find the roots
//--FIXME--    // but I have no idea how this works.
//--FIXME--
//--FIXME--    // Solve polynomial.
//--FIXME--    double roots[4];
//--FIXME--    int rootCount = solveQuartic( coeffs, roots );
//--FIXME--
//--FIXME--    // See if any of the roots are valid.
//--FIXME--    for ( j = 0; j < rootCount; j++ ) {
//--FIXME--      float dist = roots[j];
//--FIXME--
//--FIXME--      // First see if the root is in the interval of influence of
//--FIXME--      // the currently active components.
//--FIXME--      if ( dist >= intervals[i].bound &&
//--FIXME--	   dist <= intervals[i+1].bound ) {
//--FIXME--        /// Correct distance.
//--FIXME--        dist = (dist + startDist);
//--FIXME--
//--FIXME--	inter.distance = dist;
//--FIXME--	return true;
//--FIXME--      }
//--FIXME--    }
//--FIXME--  }
//--FIXME--
//--FIXME--  return false;
}

bool PrimitiveBlob::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TIntersection inter;
  if ( !PrimitiveBlob::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < distMax2 )
    return true;
  return false;
}

bool PrimitiveBlob::allIntersectionRay( const TRay &ray,
					TIntersectionStack &IStack ) const {
  int i, j;

  // Get the intervals along the ray where each component has an effect.
  int cnt;
  ((PrimitiveBlob*)this)->intervalCount = 0;
  if ( ( cnt = determineInfluence( ray, 1.0e-2f ) ) == 0 )
    // Ray doesn't hit any of the component elements.
    return false;

  // To avoid numerical problems we start at the first interval.
  float startDist;
  if ( ( startDist = intervals[0].bound ) < 0.001f )
    startDist = 0.0f;

  for ( i = 0; i < cnt; i++ )
    intervals[i].bound -= startDist;

  // Get the new starting point.
  TPoint3 P = ray.origin + ( startDist * ray.direction );

  // Clear out the coefficients.
  double coeffs[5];
  coeffs[0] =
  coeffs[1] =
  coeffs[2] =
  coeffs[3] = 0.0;
  coeffs[4] = - (double)threshold;
  
  // Step through the list of intersection points, adding the
  // influence of each component as it appears. 
  int inFlag;
  bool intersectionFound = false;
  for ( i = inFlag = 0; i < cnt; i++) {
    if ( intervals[i].type == ENTERING ) {
      // Something is just starting to influence the ray, so calculate
      // its coefficients and add them into the pot. 
      inFlag++;

      intervals[i].element->computeCoeff( P, ray.direction );
      
      for ( j = 0; j < 5; j++ )
        coeffs[j] += (double)intervals[i].element->f[j];
    }
    else {
      // We are losing the influence of a component -->
      // subtract off its coefficients. 
      
      for ( j = 0; j < 5; j++ )
        coeffs[j] -= (double)intervals[i].element->f[j];
      
      // If no components are currently affecting the ray ---> skip ahead.
      if ( --inFlag == 0 )
        continue;
    }

    // If the following intersection lies close to the current intersection
    // then first add/subtract next region before testing. [DB 7/94] 
    
    if ( ( i + 1 < cnt )
	 && ( fabsf( intervals[i].bound - intervals[i + 1].bound ) < EPS ) )
      continue;

    // Transform polynomial in a way that the interval boundaries are moved
    // to 0 and 1, i. e. the roots of interest are between 0 and 1.
    float l = intervals[i].bound;
    float w = intervals[i+1].bound - l;
    float newcoeffs[5];
    newcoeffs[0] = coeffs[0] * w * w * w * w;
    newcoeffs[1] = ( coeffs[1] + 4.0f * coeffs[0] * l ) * w * w * w;
    newcoeffs[2] = ( 3.0f * l * ( 2.0f * coeffs[0] * l + coeffs[1] ) + coeffs[2] ) * w * w;
    newcoeffs[3] = ( 2.0f * l * ( 2.0f * l * ( coeffs[0] * l + 0.75f * coeffs[1] ) + coeffs[2] ) + coeffs[3]) * w;
    newcoeffs[4] = l * ( l * ( l * ( coeffs[0] * l + coeffs[1] ) + coeffs[2] ) + coeffs[3] ) + coeffs[4];

    // Calculate coefficients of corresponding bezier curve.
    float dk[5];
    dk[0] = newcoeffs[4];
    dk[1] = newcoeffs[4] + 0.25f * newcoeffs[3];
    dk[2] = newcoeffs[4] + 0.50f * ( newcoeffs[3] + newcoeffs[2] / 12.0f );
    dk[3] = newcoeffs[4] + 0.50f * ( 0.375f * newcoeffs[3] + newcoeffs[2] + 0.125f * newcoeffs[1] );
    dk[4] = newcoeffs[4] + newcoeffs[3] + newcoeffs[2] + newcoeffs[1] + newcoeffs[0];

    // Skip this interval if the ray doesn't intersect the convex hull of the
    // bezier curve, because no valid intersection will be found.

    if ( ( dk[0] >= 0.0f && dk[1] >= 0.0f && dk[2] >= 0.0f && dk[3] >= 0.0f && dk[4] >= 0.0f ) ||
	 ( dk[0] <= 0.0f && dk[1] <= 0.0f && dk[2] <= 0.0f && dk[3] <= 0.0f && dk[4] <= 0.0f ) )
      continue;

    // Now we could do bezier clipping to find the roots
    // but I have no idea how this works.

    // Solve polynomial.
    double roots[4];
    int rootCount = solveQuartic( coeffs, roots );

    // See if any of the roots are valid.
    for ( j = 0; j < rootCount; j++ ) {
      float dist = roots[j];

      // First see if the root is in the interval of influence of
      // the currently active components.
      if ( dist >= intervals[i].bound &&
	   dist <= intervals[i+1].bound ) {
        /// Correct distance.
        dist = (dist + startDist);

	IStack.push( dist );
	intersectionFound = true;
      }
    }

    // If the blob isn't used inside a CSG and we have found at least
    // one intersection then we are ready, because all possible intersections
    // will be further away (we have a sorted list!).
    //--    if ( intersectionFound )
    //--      break;
  }

  return intersectionFound;
}

TVector3 PrimitiveBlob::normalAt( const TIntersection &inter ) const {
  float val;
  TVector3 New_Point, V1;
  TVector3 Result = TVector3::zeroVector;

  // For each component that contributes to this point, add its bit to the normal
  for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	iter != blobs.end();
	iter++ )
    (*iter)->normalAt( Result, inter.pointWorldSpace );

  val = Result.magnitudeSquared();
  
  if ( val == 0.0f )
    Result = TVector3( 1.0f, 0.0f, 0.0f );
  else
    Result.normalize();
  return Result;
}

bool PrimitiveBlob::inside( const TPoint3 &p ) const {

  if ( calculateFieldValue( p ) > threshold - insideTolerance )
    return true;
  return false;
}

float PrimitiveBlob::calculateFieldValue( const TPoint3 &p ) const {
  float density = 0.0f;
  for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	iter != blobs.end();
	iter++ )
    density = density + (*iter)->calculateFieldValue( p );

  return density;
}

int PrimitiveBlob::determineInfluence( const TRay &ray, float minDist ) const {
  int i = 0;
  for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	iter != blobs.end();
	iter++, i++ ) {
    float t0, t1;
    BlobSubObject *blob = (*iter);
    if ( blob->intersectionRay( ray, minDist, t0, t1 ) )
      ((PrimitiveBlob*)this)->insertHit( t0, t1, blob );
  }
  return intervalCount;
}

void PrimitiveBlob::insertHit( const float t0, const float t1, BlobSubObject *blob ) {
  // We are entering the component.
  intervals[intervalCount].type = ENTERING;
  intervals[intervalCount].bound = t0;
  intervals[intervalCount].element = blob;

  int k;
  for ( k = 0; t0 > intervals[k].bound; k++ );

  if ( k < intervalCount ) {
    // This hit point is smaller than one that already exists -->
    // bump the rest and insert it here.

    memmove( &intervals[k+1], &intervals[k], (intervalCount-k)*sizeof(TInterval) );
      
    //--POV_MEMMOVE(&intervals[k+1], &intervals[k], (*cnt-k)*sizeof(BLOB_INTERVAL));

    // We are entering the component.
    intervals[k].type    = ENTERING;
    intervals[k].bound   = t0;
    intervals[k].element = blob;

    intervalCount++;

    // We are exiting the component.
    intervals[intervalCount].type    = EXITING;
    intervals[intervalCount].bound   = t1;
    intervals[intervalCount].element = blob;

    for (k = k + 1; t1 > intervals[k].bound; k++);

    if ( k < intervalCount ) {
      memmove( &intervals[k+1], &intervals[k], (intervalCount-k)*sizeof(TInterval) );
      //POV_MEMMOVE(&intervals[k+1], &intervals[k], (*cnt-k)*sizeof(BLOB_INTERVAL));

      // We are exiting the component.
      intervals[k].type    = EXITING;
      intervals[k].bound   = t1;
      intervals[k].element = blob;
    }

    intervalCount++;
  }
  else {
    // Just plop the start and end points at the end of the list
    intervalCount++;
    // We are exiting the component.
    intervals[intervalCount].type    = EXITING;
    intervals[intervalCount].bound   = t1;
    intervals[intervalCount].element = blob;

    intervalCount++;
  }
}

void PrimitiveBlob::print( const char *msg ) const {
  TPrimitive::print(msg);
  printf("threshold: %5.5f\n", threshold );
  //  printf("#blobs: %d\n", numBlobs );
  for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	iter != blobs.end();
	iter++ )
    (*iter)->print( "blob:" );
}

