#include "value.h"
#include "tag.h"
#include "../text/basics.h"

#include "../world.h"
#include "../handle.h"
#include "../object.h"
#include "../all_textures.h"
#include "../all_interiors.h"
#include "../all_finishes.h"
#include "../all_cameras.h"
#include "../all_primitives.h"
#include "../all_lights.h"
#include "../all_boundings.h"
#include "../all_ambients.h"
#include "../all_pigments.h"
#include "../all_atmospherics.h"
#include "../colormap.h"
//--#include "../all_patterns.h"
#include "../animators/animator.h"
#include "../all_movers.h"
#include "../bitmap/bitmap.h"
#include "../all_normals.h"
#include "../all_turbulences.h"

//---- TWorld
bool TWorld::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, worldTags );
  switch( tagId ) {
    // objects: list of TObject
  case TAG_world_objects: {
    ValueList *l = ValueList::newList(objects.size());
    int i = 0;
    for ( vector<TObject *>::const_iterator iterO = objects.begin();
	  iterO != objects.end();
	  iterO++, i++ )
      l->elems[i] = (*iterO);
    res = Value(l);
    break;
  }
    // cameras: list of TCamera
  case TAG_world_cameras: {
    ValueList *l = ValueList::newList(cameras.size());
    int i = 0;
    for ( vector<TCamera *>::const_iterator iterC = cameras.begin();
	  iterC != cameras.end();
	  iterC++, i++ )
      l->elems[i] = (*iterC);
    res = Value(l);
    break;
  }
    // lights: list of TLight
  case TAG_world_lights: {
    ValueList *l = ValueList::newList(lights.size());
    int i = 0;
    for ( vector<TLight *>::const_iterator iterL = lights.begin();
	  iterL != lights.end();
	  iterL++, i++ )
      l->elems[i] = (*iterL);
    res = Value(l);
    break;
  }
    // currentCamera: pointer to TCamera
  case TAG_world_currentCamera: res = currentCamera; break;
    // ambient: TAmbient
  case TAG_world_ambient: res = ambient; break;
    // background: TPigment
  case TAG_world_background: res = background; break;
    // atmospheric: TAtmospheric
  case TAG_world_atmospheric: res = atmospheric; break;
    // atmosphericIOR: float
  case TAG_world_atmosphereIOR: res = &atmosphereIOR; break;
    // animator: TAnimator
  case TAG_world_animator: res = animator; break;
    // default: error
  default: return false;
  }
  return true;
}

//----THandle
bool THandle::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, handleTags );
  switch( tagId ) {
  case TAG_handle_name: res = str_dup( name ); break;
    // default: error
  default: return false;
  }
  return false;
}

//----TObject
bool TObject::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, objectTags );
  switch( tagId ) {
    // primitive: TPrimitive
  case TAG_object_primitive: res = primitive; break;
    // bounding: TBounding
  case TAG_object_bounding: res = bounding; break;
    // texture: TTexture
  case TAG_object_texture: res = texture; break;
    // default:
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TTexture
bool TTexture::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, textureTags );
  switch( tagId ) {
    // pigment: TPigment
  case TAG_texture_pigment: res = pigment; break;
    // finish: TFinish
  case TAG_texture_finish: res = finish; break;
    // interior: TInterior
  case TAG_texture_interior: res = interior; break;
    // normal: TNormal
  case TAG_texture_normal: res = normal; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool TexturePattern::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, texturePatternTags );
//--TODO  switch( tagId ) {
//--TODO    // default
//--TODO  default: return TTexture::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TInterior
bool TInterior::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, interiorTags );
  switch( tagId ) {
    // IOR: float
  case TAG_interior_ior: res = &ior; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool InteriorPattern::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, interiorPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // default
//--TODO  default: return TInterior::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TFinish
bool TFinish::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, finishTags );
  switch( tagId ) {
    // reflection: float
  case TAG_finish_reflection: res = &reflectionFactor; break;
    // transmission: float
  case TAG_finish_transmission: res = &transmissionFactor; break;
    // ambient: float
  case TAG_finish_ambient: res = &ambientFactor; break;
    // diffuse: float
  case TAG_finish_diffuse: res = &diffuseFactor; break;
    // specular: float
  case TAG_finish_specular: res = &specularFactor; break;
    // roughness: float
  case TAG_finish_roughness: res = &specularRoughness; break;
    // phong: float
  case TAG_finish_phong: res = &phongFactor; break;
    // phongSize: float
  case TAG_finish_phongSize: res = &phongSize; break;
//--TODO    // metallic: float
//--TODO  case TAG_finish_metallic: res = metallic; break;
    // brilliance: float
  case TAG_finish_brilliance: res = &brilliance; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool FinishPattern::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, finishPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // default
//--TODO  default: return TFinish::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TCamera
bool TCamera::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, cameraTags );
  switch( tagId ) {
    // location: TVector3
  case TAG_camera_location: res = &location; break;
    // lookat: TVector3
  case TAG_camera_lookat: res = &lookat; break;
    // vertical: TVector3
  case TAG_camera_vertical: res = &vertical; break;
    // fov: float
  case TAG_camera_FOV: res = &fov; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool CameraClassic::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, cameraClassicTags );
  switch( tagId ) {
    // default
  default: return TCamera::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool CameraPerspective::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, cameraPerspectiveTags );
  switch( tagId ) {
    // default
  default: return TCamera::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TPrimitive
bool TPrimitive::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveTags );
  switch( tagId ) {
    // invalid tag
  case TAG_INVALID: return THandle::qualifiedExpression( res, fieldName );
    // valid tag
  default: res = additionalInformation.isSet( tagId );
  }
  return true;
}
bool PrimitivePlane::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitivePlaneTags );
  switch( tagId ) {
    // normal: TVector3
  case TAG_primitivePlane_normal: res = &m_Normal; break;
    // distanceOrigin: float
  case TAG_primitivePlane_distance: res = &m_Distance; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveSphere::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveSphereTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveSphere_center: res = &m_Center; break;
    // radius: float
  case TAG_primitiveSphere_radius: res = &m_Radius; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveBox::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveBoxTags );
  switch( tagId ) {
    // min: TVector3
  case TAG_primitiveBox_min: res = &m_Min; break;
    // max: TVector3
  case TAG_primitiveBox_max: res = &m_Max; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveTorus::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveTorusTags );
  switch( tagId ) {
    // radius1: float
  case TAG_primitiveTorus_radius1: res = &R1; break;
    // radius2: float
  case TAG_primitiveTorus_radius2: res = &R2; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool BlobSubObject::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, blobTags );
  switch( tagId ) {
    // origin: TVector3
  case TAG_blob_origin: res = &origin; break;
    // strength: float
  case TAG_blob_strength: res = &c[2]; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool BlobSphere::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, blobSphereTags );
  switch( tagId ) {
    // radius2: float
  case TAG_blobsphere_radius2: res = &radius2; break;
    // default
  default: return BlobSubObject::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool BlobCylinder::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, blobCylinderTags );
  switch( tagId ) {
    // radius2: float
  case TAG_blobcylinder_radius2: res = &radius2; break;
    // length: float
  case TAG_blobcylinder_len: res = &len; break;
    // default
  default: return BlobSubObject::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveBlob::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveBlobTags );
  switch( tagId ) {
    // blobs: list of BlobSubObject
  case TAG_primitiveBlob_blobs: {
    ValueList *l = ValueList::newList(blobs.size());
    int i = 0;
    for ( vector<BlobSubObject *>::const_iterator iter = blobs.begin();
	  iter != blobs.end();
	  iter++, i++ )
      l->elems[i] = (*iter);
    res = Value(l);
    break;
  }
    // threshold: float
  case TAG_primitiveBlob_threshold: res = &threshold; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveCylinder::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveCylinderTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveCylinder_center: res = &center; break;
    // axis: TVector3
  case TAG_primitiveCylinder_axis: res = &axis; break;
    // radius: float
  case TAG_primitiveCylinder_radius: res = &radius; break;
    // height: float
  case TAG_primitiveCylinder_height: res = &height; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveCapsule::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveCapsuleTags );
  switch( tagId ) {
    // p1: TVector3
  case TAG_primitiveCapsule_p1: res = &p1; break;
    // p2: TVector3
  case TAG_primitiveCapsule_p2: res = &p2; break;
    // radius: float
  case TAG_primitiveCapsule_radius: res = &radius; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveCSG::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveCsgTags );
  switch( tagId ) {
    // operation: int
  case TAG_primitiveCsg_operation: res = operation; break;
    // primitives: list of TPrimitive
  case TAG_primitiveCsg_primitiveList: {
    int size = primitiveList.size();
    ValueList *l = ValueList::newList(size);
    int i = 0;
    for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	  iterP != primitiveList.end();
	  iterP++, i++ )
      l->elems[i] = (*iterP);
    res = Value(l);
    break;
  }
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveHeart::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveHeartTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveHeart_center: res = &center; break;
    // radius: float
  case TAG_primitiveHeart_radius: res = &radius; break;
    // step: float
  case TAG_primitiveHeart_step: res = &tStep; break;
    // refinement: int
  case TAG_primitiveHeart_refinement: res = refinementSteps; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveQuadric::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveQuadricTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveQuadric_center: res = &center; break;
    // radius: float
  case TAG_primitiveQuadric_radius: res = &radius; break;
    // matrix: TMatrix3
  case TAG_primitiveQuadric_A: {
    ValueList *matrix = ValueList::newList(3);
    for ( int i = 0; i < 3; i++ ) {
      ValueList *row = ValueList::newList(3);
      for ( int j = 0; j < 3; j++ )
	row->elems[j] = A( j, i );
      matrix->elems[i] = row;
    }
    res =  Value(matrix);
    break;
  }
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveJulia::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveJuliaTags );
  switch( tagId ) {
    // param: TVector4
  case TAG_primitiveJulia_param: res = juliaParm; break;
    // slice: couple ( TVector4, float )
  case TAG_primitiveJulia_slice: {
    ValueList *couple = ValueList::newList(2);
    couple->elems[0] = slice;
    couple->elems[1] = sliceDist;
    res =  Value(couple);
    break;
  }
    // exitValue: float
  case TAG_primitiveJulia_exitValue: res = &exitValue; break;
    // n: int
  case TAG_primitiveJulia_n: res = n; break;
    // precision: float
  case TAG_primitiveJulia_precision: res = &precision; break;
    // center: TVector3
  case TAG_primitiveJulia_center: res = &center; break;
    // radius: float
  case TAG_primitiveJulia_radius2: res = sqrtf(radius2); break;
    //--    // exponent: TVector2
    //--  case TAG_primitiveJulia_exponent: res = exponent; break;
    // type: int
  case TAG_primitiveJulia_type: res = algebra; break;
    // subtype: string
  case TAG_primitiveJulia_subType: res = subType; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MetaSubObject::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, metaTags );
  switch( tagId ) {
    // loc: TVector3
  case TAG_meta_loc: res = &loc; break;
    // strength: float
  case TAG_meta_strength: res = &strength; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MetaSphere::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, metaSphereTags );
  switch( tagId ) {
    // default
  default: return MetaSubObject::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MetaCylinder::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, metaCylinderTags );
  switch( tagId ) {
    // default
  default: return MetaSubObject::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MetaPlane::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, metaPlaneTags );
  switch( tagId ) {
    // slant: float
  case TAG_metaplane_slant: res = &slant; break;
    // default
  default: return MetaSubObject::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveMetaObjects::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveMetaTags );
  switch( tagId ) {
    // threshold: float
  case TAG_primitiveMeta_threshold: res = &threshold; break;
    // subs: list of MetaSubObject
  case TAG_primitiveMeta_subs: {
    ValueList *l = ValueList::newList(num_subs);
    for ( int i = 0; i < num_subs; i++ )
      l->elems[i] = subs[i];
    res = Value(l);
    break;
  }
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveTransformed::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveTransformedTags );
  switch( tagId ) {
  case TAG_primitiveTransformed_primitive: res = primitive; break;
  case TAG_primitiveTransformed_transformation: {
    ValueList *m = ValueList::newList(4);
    for ( int j = 0; j < 4; j++ ) {
      ValueList *row = ValueList::newList(4);
      for ( int i = 0; i < 4; i++ )
	row->elems[i] = transform.matrix( i, j );
      m->elems[j] = row;
    }
    res =  Value(m);
    break;
  }
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveSuperellipsoid::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveSuperellipsoidTags );
  switch( tagId ) {
    // e: float
  case TAG_primitiveSuperellipsoid_e: res = &e; break;
    // n: float
  case TAG_primitiveSuperellipsoid_n: res = &n; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveSor::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveSorTags );
  switch( tagId ) {
    //--TODO    // points: list of TVector2
    //--TODO  case TAG_primitiveSor_points: {
    //--TODO    break;
    //--TODO  }
    //--TODO    // open: bool
    //--TODO  case TAG_primitiveSor_open: res = open; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PrimitiveCone::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, primitiveConeTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveCone_center: res = &center; break;
    // axis: TVector3
  case TAG_primitiveCone_axis: res = &axis; break;
    // angle: float
  case TAG_primitiveCone_angle: res = &angle; break;
    // default
  default: return TPrimitive::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TLight
bool TLight::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, lightTags );
  switch( tagId ) {
    // color: TColor
  case TAG_light_color: res = &color; break;
    // brightness: float
  case TAG_light_brightness: res = &brightness; break;
//--TODO    // powerOn: boolean
//--TODO  case TAG_light_powerOn: res = powerOn; break;
//--TODO    // looksLike: TPrimitive
//--TODO  case TAG_light_looksLike: res = looksLike; break;
    // volume: TPrimitive
  case TAG_light_volume: res = volume; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool LightDot::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, lightDotTags );
  switch( tagId ) {
    // location: TVector3
  case TAG_lightDot_location: res = &location; break;
    // attenuation: boolean
  case TAG_lightDot_attenuation: res = attenuation; break;
    // default
  default: return TLight::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool LightSquare::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, lightSquareTags );
  switch( tagId ) {
    //--    // location: TVector3
    //--  case TAG_lightSquare_location: res = &location; break;
    // dir: TVector3
  case TAG_lightSquare_dir: res = &dir; break;
    // axis: couple of TVector3
  case TAG_lightSquare_axis: {
    ValueList *l = ValueList::newList(2);
    l->elems[0] = &axis1;
    l->elems[1] = &axis2;
    res = Value(l);
    break;
  }
    // numdots: couple of int
  case TAG_lightSquare_numdots: {
    ValueList *l = ValueList::newList(2);
    l->elems[0] = numDots1;
    l->elems[1] = numDots2;
    res = Value(l);
    break;
  }
  case TAG_lightSquare_jitter: res = jitter; break;
    // default
  default: return LightDot::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool LightSpherical::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, lightSphericalTags );
//--TODO  switch( tagId ) {
//--TODO    // diameter: float
//--TODO  case TAG_lightSpherical_diameter: res = diameter; break;
//--TODO    // default: call LightDot::qualifiedExpression
//--TODO  default: return LightDot::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TBounding
bool TBounding::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, boundingTags );
  switch( tagId ) {
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool BoundingAABB::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, boundingAABBTags );
  switch( tagId ) {
    // min: TVector3
  case TAG_boundingAABB_min: res = &m_Min; break;
    // max: TVector3
  case TAG_boundingAABB_max: res = &m_Max; break;
    // default
  default: return TBounding::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool BoundingSphere::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, boundingSphereTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_boundingSphere_center: res = m_Center; break;
    // radius: float
  case TAG_boundingSphere_radius2: res = m_Radius2; break;
    // default
  default: return TBounding::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool BoundingSlabs3::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, boundingSlabsTags );
//--TODO  switch( tagId ) {
//--TODO    // slabs: triple of Triple ( TVector3, float, float )
//--TODO  case TAG_boundingSlabs_slabs: {
//--TODO    ValueList *l = ValueList::newList(3);
//--TODO    for ( int i = 0; i < 3; i++ ) {
//--TODO      ValueList *triple = ValueList::newList(3);
//--TODO      triple->elems[0] = slabs[i].direction;
//--TODO      triple->elems[1] = slabs[i].min;
//--TODO      triple->elems[2] = slabs[i].max;
//--TODO      l->elems[i] = triple;
//--TODO    }
//--TODO    res = Value(l);
//--TODO    break;
//--TODO  }
//--TODO    // default
//--TODO  default: return TBounding::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TAmbient
bool TAmbient::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, ambientTags );
  switch( tagId ) {
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool AmbientSolid::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, ambientSolidTags );
  switch( tagId ) {
    // color: TColor
  case TAG_ambientSolid_color: res = &color; break;
    // default
  default: return TAmbient::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool AmbientHemispherical::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, ambientHemisphericalTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_ambientHemispherical_center: res = &center; break;
    // up: TVector3
  case TAG_ambientHemispherical_up: res = &up; break;
    // colorTop: TColor
  case TAG_ambientHemispherical_colorTop: res = &colorTop; break;
    // colorBottom: TColor
  case TAG_ambientHemispherical_colorBottom: res = &colorBottom; break;
    // default
  default: return TAmbient::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TPigment
bool TPigment::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentTags );
  switch( tagId ) {
    // anchor: TVector3
  case TAG_pigment_anchor: res = &anchor; break;
    // simple: TColor
  case TAG_pigment_simple: res = &simple; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentSolid::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentSolidTags );
  switch( tagId ) {
    // color: TColor
  case TAG_pigmentSolid_color: res = &color; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentNoisy::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentNoisyTags );
  switch( tagId ) {
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentSkymap::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentSkymapTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentSkymap_map: res = map; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentChecker::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentCheckerTags );
  switch( tagId ) {
    // black: TColor
  case TAG_pigmentChecker_black: res = &colorBlack; break;
    // white: TColor
  case TAG_pigmentChecker_white: res = &colorWhite; break;
    // size: TVector3
  case TAG_pigmentChecker_size: res = &size; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentGranite::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentGraniteTags );
  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentGranite_map: res = map; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentCubemap::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentCubemapTags );
  switch( tagId ) {
    // maps: list of 6 TBitmapColor
  case TAG_pigmentCubemap_maps: {
    ValueList *l = ValueList::newList(6);
    for ( int i = 0; i < 6; i++ )
      l->elems[i] = maps[i];
    res = Value(l);
    break;
  }
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentCylindrical::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentCylindricalTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentCylindrical_map: res = map; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentSpherical::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentSphericalTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentSpherical_map: res = map; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentTorusmap::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentTorusmapTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentTorusmap_map: res = map; break;
    // gradient: TVector3
  case TAG_pigmentTorusmap_gradient: res = &gradient; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentPlanar::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentPlanarTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentPlanar_map: res = map; break;
    // gradient: TVector3
  case TAG_pigmentPlanar_gradient: res = &gradient; break;
    // size: float
  case TAG_pigmentPlanar_size: res = &size; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentMandel::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentMandelTags );
  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentMandel_map: res = map; break;
    // iterations: int
  case TAG_pigmentMandel_iterations: res = iterations; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentMarble::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentMarbleTags );
  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentMarble_map: res = map; break;
    // turbulence: TTurbulence
  case TAG_pigmentMarble_turbulence: res = turbulence; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool PigmentWood::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, pigmentWoodTags );
  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentWood_map: res = map; break;
    // turbulence: TTurbulence
  case TAG_pigmentWood_turbulence: res = turbulence; break;
    // default
  default: return TPigment::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TAtmospheric
bool TAtmospheric::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, atmosphericTags );
  switch( tagId ) {
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool AtmosphericFog::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, atmosphericFogTags );
  switch( tagId ) {
    // density: float
  case TAG_atmosphericFog_density: res = &density; break;
    // color: TColor
  case TAG_atmosphericFog_color: res = &color; break;
    // filter: float
  case TAG_atmosphericFog_filter: res = &filter; break;
    // turbulence: TTurbulence
  case TAG_atmosphericFog_turbulence: res = turbulence; break;
    // depth: float
  case TAG_atmosphericFog_turbulenceDepth: res = &turbulenceDepth; break;
    // default
  default: return TAtmospheric::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool AtmosphericGround::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, atmosphericGroundTags );
  switch( tagId ) {
    // offset: float
  case TAG_atmosphericGround_offset: res= &offset; break;
    // altitude: float
  case TAG_atmosphericGround_altitude: res = &altitude;
    // up: TVector3
  case TAG_atmosphericGround_up: res = &up;
    // default
  default: return AtmosphericFog::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool AtmosphericVolumetric::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, atmosphericVolumetricTags );
  switch( tagId ) {
    // volume: TPrimitive
  case TAG_atmosphericVolumetric_volume: res = volume; break;
    // default
  default: return AtmosphericFog::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TColormap
bool TColorMap::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, colormapTags );
  switch( tagId ) {
    // map: list of couple ( startValue, color )
  case TAG_colormap_map: {
    ValueList *l = ValueList::newList( nbItems );
    for ( int i = 0; i < nbItems; i++ ) {
      ValueList *couple = ValueList::newList(2);
      couple->elems[0] = map[i].startValue;
      couple->elems[1] = map[i].color;
      l->elems[i] = Value(couple);
    }
    res = Value(l);
    break;
  }
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}

//--TODO//----TPattern
//--TODObool TPattern::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, patternTags );
//--TODO  switch( tagId ) {
//--TODO  case TAG_pattern_anchor: res = anchor; break;
//--TODO    // default
//--TODO  default: return THandle::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}
//--TODObool PatternMap::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, patternMapTags );
//--TODO  switch( tagId ) {
//--TODO    // map: list of couple ( startValue, handle )
//--TODO  case TAG_patternmap_map: {
//--TODO    ValueList *l = ValueList::newList( nbItems );
//--TODO    for ( int i = 0; i < nbItems; i++ ) {
//--TODO      ValueList *couple = ValueList::newList(2);
//--TODO      couple->elems[0] = map[i].startValue;
//--TODO      couple->elems[1] = (THandle*)map[i].handle;
//--TODO      l->elems[i] = Value(couple);
//--TODO    }
//--TODO    res = Value(l);
//--TODO    break;
//--TODO  }
//--TODO    // default
//--TODO  default: return THandle::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}
//--TODObool PatternCheckboard::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, patternCheckboardTags );
//--TODO  switch( tagId ) {
//--TODO    // white: THandle
//--TODO  case TAG_patternCheckboard_white: res = (THandle*)white; break;
//--TODO    // black: THandle
//--TODO  case TAG_patternCheckboard_black: res = (THandle*)black; break;
//--TODO    // size: TVector3
//--TODO  case TAG_patternCheckboard_size: res = size; break;
//--TODO    // default
//--TODO  default: return TPattern::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TAnimator
bool TAnimator::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, animatorTags );
  switch( tagId ) {
    // movers: list of TMover
  case TAG_animator_movers: {
    ValueList *l = ValueList::newList(movers.size());
    int i = 0;
    for ( vector<TMover*>::const_iterator iter = movers.begin();
	  iter != movers.end();
	  iter++ )
      l->elems[i] = (*iter);
//--    ValueList *l = ValueList::newList(numMovers);
//--    for ( int i = 0; i < l->size; i++ )
//--      l->elems[i] = movers[i];
    res = Value(l);
    break;
  }
    // speed: float
  case TAG_animator_speed: res = &speed; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TMover
bool TMover::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverTags );
  switch( tagId ) {
    // dest: list of float/vector3 pointer
  case TAG_mover_dest: {
    ValueList *l = ValueList::newList(dest.size());
    int i = 0;
    for ( vector<void*>::const_iterator iter = dest.begin();
	  iter != dest.end();
	  iter++, i++ )
      if ( vMove )
	l->elems[i] = *((TVector3*)(*iter));
      else
	l->elems[i] = *((float*)(*iter));
//--    ValueList *l = ValueList::newList(numDest);
//--    for ( int i = 0; i < l->size; i++ )
//--      if ( vMove )
//--	l->elems[i] = *((TVector3*)dest[i]);
//--      else
//--	l->elems[i] = *((float*)dest[i]);
    res = Value(l);
    break;
  }
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverAddV::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverAddTags );
  switch( tagId ) {
    //    // source1: TVector3 ptr
    //  case TAG_moverAdd_source1: res = source1; break;
    //    // source2: TVector3 ptr
    //  case TAG_moverAdd_source2: res = source2; break;
    // source: list of TVector3 ptr
  case TAG_moverAdd_source: {
    int size = source.size();
    ValueList *l = ValueList::newList(size);
    int i = 0;
    for ( vector<TVector3*>::const_iterator iter = source.begin();
	  iter != source.end();
	  iter++, i++ )
      l->elems[i] = (*iter);
    res = Value(l);
    break;
  }
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverCircle::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverCircleTags );
  switch( tagId ) {
    // source: TVector3
  case TAG_moverCircle_source: res = &source; break;
    // axis: TVector3
  case TAG_moverCircle_axis: res = &axis; break;
    // speed: float
  case TAG_moverCircle_speed: res = &speed; break;
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverLinearF::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverLinearFTags );
  switch( tagId ) {
    // zero: float
  case TAG_moverLinearF_zero: res = &zero; break;
    // factor: float
  case TAG_moverLinearF_factor: res = &fact; break;
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverBounce::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverBounceTags );
  switch( tagId ) {
    // gravity: float
  case TAG_moverBounce_gravity: res = &gravity; break;
    // speed: float
  case TAG_moverBounce_speed: res = &speed; break;
    // height: float
  case TAG_moverBounce_height: res = &height; break;
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverLissajou::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverLissajouTags );
  switch( tagId ) {
    // pulse: TVector3
  case TAG_moverLissajou_pulse: res = &pulse; break;
    // phase: TVector3
  case TAG_moverLissajou_phase: res = &phase; break;
    // amplitude: TVector3
  case TAG_moverLissajou_amplitude: res = &ampl; break;
    // shift: TVector3
  case TAG_moverLissajou_shift: res = &shift; break;
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool MoverBezier::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, moverBezierTags );
  switch( tagId ) {
    // segments: list of triple ( TVector3, TVector3, float )
  case TAG_moverBezier_segments: {
    ValueList *l = ValueList::newList( numSegments );
    for ( int i = 0; i < numSegments; i++ ) {
      ValueList *triplet = ValueList::newList( 3 );
      triplet->elems[0] = segments[i].point;
      triplet->elems[1] = segments[i].vector;
      triplet->elems[2] = segments[i].time;
      l->elems[i] = Value(triplet);
    }
    res = Value(l);
    break;
  }
    // default
  default: return TMover::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TBitmapColor
bool TBitmapColor::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, bitmapTags );
  switch( tagId ) {
  case TAG_bitmap_width: res = width; break;
  case TAG_bitmap_height: res = height; break;
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}

//----TNormal
bool TNormal::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, normalTags );
  switch( tagId ) {
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool NormalBumps::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, normalBumpsTags );
  switch( tagId ) {
    // bumps: float
  case TAG_normalBumps_bumps: res = &bumps; break;
    // default
  default: return TNormal::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool NormalTurbulence::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, normalTurbulenceTags );
  switch( tagId ) {
    // factor: float
  case TAG_normalTurbulence_factor: res = &factor; break;
    // turbulence: TTurbulence
  case TAG_normalTurbulence_turbulence: res = turbulence; break;
    // default
  default: return TNormal::qualifiedExpression( res, fieldName );
  }
  return true;
}
//--TODObool NormalPattern::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, normalPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // default
//--TODO  default: return TNormal::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}
//--TODObool NormalFunction::qualifiedExpression( Value &res, const char *fieldName ) const {
//--TODO  int tagId = TTagList::findId( fieldName, normalFunctionTags );
//--TODO  switch( tagId ) {
//--TODO    // default
//--TODO  default: return TNormal::qualifiedExpression( res, fieldName );
//--TODO  }
//--TODO  return true;
//--TODO}

//----TTurbulence
bool TTurbulence::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, turbulenceTags );
  switch( tagId ) {
    // turbulence: TVector3
  case TAG_turbulence_turbulence: res = &turbulence; break;
    // octaves: int
  case TAG_turbulence_octaves: res = octaves; break;
    // lambda: float
  case TAG_turbulence_lambda: res = &lambda; break;
    // omega: float
  case TAG_turbulence_omega: res = &omega; break;
    
    // default
  default: return THandle::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool TurbulenceNoise::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, turbulenceNoiseTags );
  switch( tagId ) {
    // default
  default: return TTurbulence::qualifiedExpression( res, fieldName );
  }
  return true;
}
bool TurbulencePerlin::qualifiedExpression( Value &res, const char *fieldName ) const {
  int tagId = TTagList::findId( fieldName, turbulencePerlinTags );
  switch( tagId ) {
    // default
  default: return TTurbulence::qualifiedExpression( res, fieldName );
  }
  return true;
}
