!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!
take transformation from povray and don't use TMatrix4, use a new type TTransform

OCTREE
.octree intersection (traversal)
.octree bugs when inserting an object overlapped by an octree node bounding box

BSP
.when traversing bsp/octree, an object may be in 2 or more leaf; so an object could be test twice or more -> store in object a bit telling if it has already been tested

OPTIMIZATION
.sweep plane: in world.precalcFrame, we compute intersections between sweep plane and primitives

INTERSECTION
.Box intersections: each primitive except box
.Metaobject: allIntersections

INSIDE
.metaobject

NORMAL
.heart is wrong (when (IP - center).y < 0 )

PIGMENT
.cubemap: load 6 maps
          cubemap doesn't fit very well for non-cubic box -> a transform vector/matrix?

PARSER
.when a sub-handle doesn't have any specific tag (MetaSphere, TAmbient...), we don't need
  to declare qualifiedExpression and parse (parent method will automatically be called)

MEMORY
.when calling THandle::operator delete, the destructor is also called and then pointers may be nullified
  -> segmentation fault
.find the memory leak
.3 kind of rays:
  .basicRay: origin, direction, reverseDirection (pure virtual)
  .lightRay(:basicRay): target (used by findFirstIntersection and intersectsRay)
  .Ray(:basicRay): intersection, reflected, transmitted, lightray, insideList (used by traceRay, findClosestIntersection and intersectionRay)

OTHER
.transformation: y scaling > 1  doesn't work -> result is a translation ????
.use intersection.pointWorldSpace and intersection.pointObjectSpace
.renderSimple for each primitives (box already done)
.new primitives: height field (from povray), meshes, cones
.use PCX header and read different type of pcx
.problem when trying to find intersection with PrimitiveTransformed bounding box
 because bounding box doesn't have access to primitive transformation matrix
 2 solutions: .find a way to access transformation matrix from bounding box
              .when calling primitive setup, modify bounding box according to transformation matrix
.supersampling: adaptative quincunx (!!step must be a divider of width and height!!)
                 interpolate intersection in function of primitive then compute lighting
                 or compute lighting at square corner, then interpolate using blend2, 4, 8, 16
                generalize rgssNxN (must solve queen row/column problem)
.stop using dynamic casting; primitives/lights/boundings/... have an additional info: kind (set in constructor) general intersectionRay, intersectsRay, ... calling the right method base on kind info
.in RendererInterpolated::vertex, we could store tex coord, reflect col, or intersection point
.fog attenuates intensity of primitives in fog -> if fog attenuation is below a 1-MIN_ATTENUATION
  we don't shoot reflected and transmitted ray => compute fog density before diffuse, reflected, transmitted
.use whichRendering
.double_illumination: A surface has two sides; usually, only the side facing the light source is illuminated, the other side remains in shadow. When double_illuminate is used, the other side is also illuminated. This is useful for simulating effects like translucency (as in a lamp shade, sheet of paper, etc).  [double_illuminate only illuminates both sides of the same surface, so on a sphere, for example, you will not see the effect unless the sphere is either partially transparent, or if the camera is inside and the light source outside of the sphere (or vise versa).]
.replace mapping pigment with a unique pigment: PigmentMapped with a pointer to a new structure:
  TMapping: public THandle   method: void mapPoint( const TPoint3 &p, float &x, float &y ) const;
  its derivates will redefine this method
 PigmentMapped::getColor( const TPoint3 &p ) const {
   float x, y;
   mapping->mapPoint( p, x, y );
   return map->dotAt( x, y ); // dependent on whichMapping
 }
 TMapping can also be used in TNormal for bump mapping
 mapping is not uniform, we sometimes clip in mapping and in dotAt
.use BSphereTree in blob
.use BBoxTree in mesh and world
.new primitive method: computeAABB
.sor: normal is sometimes miscalculated (normal[__Y] sign suddenly changes) same case as below
      intersection is wrong when |direction[__Y]|<EPS
.macro in scene file:
   macro sphereMacro(x) {
     object {
       primitive = primitive: sphere {
         center = <x,0,0>;
         radius = 1;
       };
       texture = whiteTexture;
     }
  }
  world {
   // world will contains 3 unit sphere with center 0,0,0  2,0,0  and  4,0,0
   objects = [[ sphereMacro(0), sphereMacro(2), sphereMacro(4) ]];
  }

PROFILING
.don't return TColor, TVector3... replace with in/out parameter -> no more copy constructor (10% CPU)
.computeLighting takes 45% CPU time (12.8% itself and rest for child)

rendering: NORMAL (1 sample/point)
1522 ticks -> 1.52200 sec. to generate
rendering: BILINEAR (0.5 sample/point)
380 ticks -> 0.38000 sec. to generate
rendering: QUINCUNX (5 samples/point  2 computed)
3044 ticks -> 3.04400 sec. to generate
rendering: FLIPQUAD (4 samples/point  2 computed)
3065 ticks -> 3.06500 sec. to generate
rendering: GRID2X2 (4 samples/point  4 computed)
5868 ticks -> 5.86800 sec. to generate
rendering: CORNER (4 samples/point  1 computed)
1563 ticks -> 1.56300 sec. to generate
rendering: DIAMOND (4 samples/point  2 computed)
2954 ticks -> 2.95400 sec. to generate
rendering: RGSS2x2 (4 samples/point  4 computed)
5758 ticks -> 5.75800 sec. to generate
rendering: QUINCUNX ADAPTATIVE COLOR (max 1 sample/point)
861 ticks -> 0.86100 sec. to generate
rendering: QUINCUNX ADAPTATIVE PRIMITIVE (max 1 sample/point)
100 ticks -> 0.10000 sec. to generate
rendering: INTERPOLATED (max 1 sample/point)
191 ticks -> 0.19100 sec. to generate
