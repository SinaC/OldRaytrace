#include <stdio.h>

#include "../world.h"
#include "../ray.h"
#include "metaobjects.h"

//#define __DEBUG__
//#define __DEBUG2__

const TTagList PrimitiveMetaObjects::primitiveMetaTags [] = {
  { "threshold", TAG_primitiveMeta_threshold },
  { "meta", TAG_primitiveMeta_subs },
  { NULL, -1 }
};

///////////////
// Meta Basic
//
const TTagList MetaSubObject::metaTags [] = {
  { "location", TAG_meta_loc },
  { "strength", TAG_meta_strength },
  { NULL, -1 }
};

const TTagList MetaSubObject::metaSubClassTags [] = {
  { "plane", META_PLANE },
  { "sphere", META_SPHERE },
  { "cylinder", META_CYLINDER },
  {  NULL, -1 }
};

MetaSubObject::MetaSubObject() {
  THandle::THandle(); handleKind = HANDLE_META;
  metaKind = META_NOTHING;
}

void MetaSubObject::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", metaName() );
  loc.print("loc:");
  printf("strength: %5.5f\n", strength );
}

char *MetaSubObject::metaName() const {
  for ( int i = 0; metaSubClassTags[i].tagName != NULL; i++ )
    if ( metaSubClassTags[i].tagId == metaKind )
      return metaSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", metaKind );
  return buf;
}

///////////////
// MetaSphere
//
const TTagList MetaSphere::metaSphereTags [] = {
  { NULL, -1 }
};
MetaSphere::MetaSphere()
: MetaSubObject() {
  metaKind = META_SPHERE;
}
float MetaSphere::density( float x, float x2 ) const {
  return strength / ( x2 + B*x + C );
}
void MetaSphere::getMax( float &max_p, float &max_D, const TRay &ray ) {
  B = ray.direction | ( loc - ray.origin );
  C = ( loc - ray.origin ).magnitudeSquared() + 0.001f;
  max_p = B;
  max_D = strength / ( C-B*B );
  B = -2*B;
}
void MetaSphere::addGrad( TVector3 &res, const TVector3 &hit ) {
	const TVector3 hit2loc = hit - loc;
	float n = hit2loc.magnitudeSquared();
	float c = strength / ( n*n ) ;
	res += hit2loc * c;
}
void MetaSphere::print( const char *msg ) const {
  MetaSubObject::print(msg);
}

///////////////
// MetaCylinder
//
const TTagList MetaCylinder::metaCylinderTags [] = {
  { NULL, -1 }
};
MetaCylinder::MetaCylinder()
: MetaSubObject() { 
	metaKind = META_CYLINDER;
}
float MetaCylinder::density( float x, float x2 ) const {
  return strength / ( A*x2 + B*x + C );
}
void MetaCylinder::getMax( float &max_p, float &max_D, const TRay &ray ) {
  A = SQR( ray.direction[__X] ) + SQR( ray.direction[__Z] );
  
  if ( A > 0.001f ) {
    B = ray.direction[__X] * ( loc[__X] - ray.origin[__X] ) +
      ray.direction[__Z] * ( loc[__Z] - ray.origin[__Z] );
    
    C = ( ray.origin[__X] - loc[__X] ) * ( ray.origin[__X] - loc[__X] ) +
      ( ray.origin[__Z] - loc[__Z] ) * ( ray.origin[__Z] - loc[__Z] ) + 0.01f;
      
    max_p = B / A;
    max_D = strength / ( C-B*B/A );
    B = -2*B;
  }
  
}
void MetaCylinder::addGrad( TVector3 &res, const TVector3 &hit ) {
  float n = SQR( hit[__X] - loc[__X] ) + SQR( hit[__Z] - loc[__Z] );
  float c = strength / ( n*n );
  res[__X] += c*( hit[__X] - loc[__X] );
  res[__Z] += c*( hit[__Z] - loc[__Z] );
}
void MetaCylinder::print( const char *msg ) const {
  MetaSubObject::print(msg);
}

///////////////
// MetaPlane
//
const TTagList MetaPlane::metaPlaneTags [] = {
  { "slant", TAG_metaplane_slant },
  { NULL, -1 }
};
MetaPlane::MetaPlane()
: MetaSubObject() {
  metaKind = META_PLANE;
}
float MetaPlane::density( float x, float x2 ) const {
  return strength / ( A*x2 + B*x + C );
}

void MetaPlane::getMax( float &max_p, float &max_D, const TRay &ray ) {
  A = SQR( ray.direction[__Y] );
  B = ray.direction[__Y] * (loc[__X] - ray.origin[__Y] );  
  C = ( ray.origin[__Y] - loc[__Y]) * ( ray.origin[__Y] - loc[__Y] ) + slant;
  
  if ( A > 0.001f ) {
    max_p = B / A;
    max_D = 1.0f / slant;
  }
  else {
    max_p = 0;
    max_D = 1.0f / ( A + slant );
  }
  B = -2*B;
}

void MetaPlane::addGrad( TVector3 &res, const TVector3 &hit ) {
  float n = SQR( hit[__Y] -loc[__Y] );
  float c = strength / ( n*n );
  res[__Y] += c*( hit[__Y] - loc[__Y] );
}
void MetaPlane::print( const char *msg ) const {
  MetaSubObject::print(msg);
  printf("slant: %5.5f\n", slant );
}

///////////////////////////////////////////
// Meta ObjectS
//

PrimitiveMetaObjects::PrimitiveMetaObjects() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_META;
  num_subs = 0;
}

PrimitiveMetaObjects::~PrimitiveMetaObjects() {
  for ( int i = 0; i < num_subs; i++ )
    subs[i] = NULL;
  num_subs = 0;
}

float PrimitiveMetaObjects::density( float x ) const {  
  float res = -threshold;

  float x2 = x*x;

  for ( MetaSubObject*const* p = subs; p < subs+num_subs; p++ )
    res += (*p)->density( x, x2);
  
  return res;
}

bool PrimitiveMetaObjects::intersectionRay( const TRay &ray,
					    TIntersection &inter ) const {
  float first = INFINITE, last = -INFINITE, ubound=-threshold;  

  for ( MetaSubObject*const* p = subs; p < subs+num_subs; p++ ) {
    float m, M;
    (*p)->getMax( m, M, ray );
    
    ubound += M;
		
    if ( m < first )
      first = m;
    if ( m > last )
      last = m;
  }

  if ( ubound < 0 )
    return false;

#ifdef __DEBUG__
  printf("--compute intersection--\n");
  ray.origin.print("ray.origin");
  ray.direction.print("ray.direction");
  printf("[%2d] first: %5.5f[%5.5f]  last: %5.5f[%5.5f]  uBound: %5.5f  0[%5.5f]\n",
	 ray.kind, first, density(first), last, density(last), ubound, density(0) );
#endif

  float x0, x1;
  const float epsilon = 0.1f;
  const float step = 1.0f;
  // if density(0) is positive, ray starts from inside the metaobject
  if ( density(0) >= 0.0f ) {
    // last is always positive
    if ( first < 0 ) // smallest sub is before origin
      x0 = 0; // starts from origin
    else
      x0 = first; // starts from first
    // density(x0) is always positive
    // x0 and x1 will be advance until density(x0)>0 and density(x1)<0
    // x1 starts to x0+step
    x1 = x0+step;
#ifdef __DEBUG__
    printf("starting x0:%5.5f[%5.5f]  x1:%5.5f[%5.5f]\n",
	   x0, density(x0), x1, density(x1) );
#endif
    while ( density( x1 ) > 0 ) {
      //--      if ( x1 > last ) // no intersection if step over last
      //--	return false;
      x0 = x1; // starts from previous end
      x1 += step; // ends advances
    }
#ifdef __DEBUG__
    printf("x0,x1 computed x0:%5.5f[%5.5f]  x1:%5.5f[%5.5f]\n",
	   x0, density(x0), x1, density(x1) );
#endif
    goto search;
  }

  // if ray doesn't start from inside, last cannot be before ray origin
  if ( last < 0 )
    return false;
  
  if ( first < 0 ) // closest sub is before origin
    x1 = step; // ends to step (close to origin)
  else {
    if ( density( first ) > 0 ) { // if first is inside
      x1 = first; // ends to first
      x0 = 0;     // starts from origin
      goto search;
    }
    else // if first is outside
      x1 = first+step; // ends to first+step
  }
  // we only know end -> compute start
  while ( x1 < last ) { // while end point is before last
    if ( density( x1 ) > 0 ) { // if end is inside
      x0 = x1-step; // starts from end-step
      goto search;
    }
    x1 += step; // if end is outside, ends to end+step (advance end)
  }
#ifdef __DEBUG__
  printf("leaving  x0: %5.5f[%5.5f]  x1: %5.5f[%5.5f]\n",
	 x0, density(x0), x1, density(x1) );
#endif
  // no intersection if ends after last
  return false;
  // 2 main cases:
  //  x0 = 0        x1 = first
  //  x0 = x1-step  x1 >= step
 search:
#ifdef __DEBUG__
  printf("search  x0:%5.5f[%5.5f]  x1:%5.5f[%5.5f]\n",
	 x0, density(x0), x1, density(x1) );
#endif
  
  // dichotomy to find a better root
  do {
    float mid = ( x1 + x0 ) * 0.5f;
    if ( density( mid ) > 0 )
      x1 = mid;
    else
      x0 = mid;
  } while ( x1-x0 > epsilon );
  inter.distance = ( x1 + x0 ) * 0.5f;

#ifdef __DEBUG__
  printf("inter.distance: %5.5f[%5.5f]  x0:%5.5f[%5.5f]  x1:%5.5f[%5.5f]\n",
	 inter.distance, density(inter.distance), 
	 x0, density(x0), x1, density(x1) );
  (ray.origin+(inter.distance*ray.direction)).print("intersection:");
#endif

  return true;
}

TVector3 PrimitiveMetaObjects::normalAt( const TIntersection &inter ) const {
  TVector3 result = TVector3::zeroVector;

  for ( MetaSubObject*const* p = subs; p < subs+num_subs; p++ )
    (*p)->addGrad( result, inter.pointWorldSpace );

  result.normalize();

  #ifdef __DEBUG2__
  //inter.ray->direction.print("====>DIRECT");
  result.print("====>NORMAL:");
#endif


  return result;
}

bool PrimitiveMetaObjects::intersectsRay( const TRay &ray, float distMax2 ) const {
  TIntersection inter;
  if ( !intersectionRay( ray, inter ) )
    return false;
  return inter.distance * inter.distance < distMax2;
    // FIXME

  TRay r;
  
  r.direction = ray.direction;
  r.origin = ray.target;

  float first = INFINITE, last = -INFINITE, ubound = -threshold;
  
  for (MetaSubObject*const* p = subs; p < subs+num_subs; p++ ) {
    float m, M;
    (*p)->getMax( m, M, r );
    
    ubound += M;
		
    if ( m < first )
      first = m;
    if ( m > last )
      last = m;
  }

  float x1, x0;
  const float step = 0.5f;
  const float epsilon = 0.1f;
  // only used in next while loop but must be defined cos' of goto
  const float lStep = sqrtf(distMax2) + step - 0.5f;//a small delta should make it

  if ( first < 0 )
    x1 = step;
  else {
    if ( density( first ) > 0 ) {
      x1 = first;
      x0 = 0;
      goto search;
    }
    else
      x1 = first+step;
  }
  
  //--FIXME  while ( x1 < (last <? l+step ) ) {
  //--  while ( x1 < last ) {
  while ( x1 < ( (last < lStep)?last:lStep) ) {
    if ( density( x1 ) > 0 ) {
      x0 = x1-step;
      goto search;
    }
    x1 += step;
  }
  return false;
  
 search:
  do {
    float mid = ( x1 + x0 ) * 0.5f;
    if ( density( mid ) > 0 )
      x1 = mid;
    else
      x0 = mid;
  } while ( x0*x0 < distMax2
	    && x1*x1 > distMax2
	    && x1-x0 > epsilon );// should I keep the 3rd test ???
  return ( x1 + x0 ) * ( x1 + x0 ) * 0.25f < distMax2;
}

bool PrimitiveMetaObjects::allIntersectionRay( const TRay &ray,
					       TIntersectionStack &IStack ) const {
  return false;
}

void PrimitiveMetaObjects::print( const char *msg ) const {
  TPrimitive::print( msg );
  printf("threshold: %5.5f\n", threshold );
  printf("number of subs: %d\n", num_subs );
  for ( int i = 0; i < num_subs; i++ )
    subs[i]->print( "sub:" );
}
