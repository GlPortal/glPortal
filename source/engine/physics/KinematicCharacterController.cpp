/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2008 Erwin Coumans  http://bulletphysics.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote
the original software. If you use this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as
being the original software.
3. This notice may not be removed or altered from any source distribution.

-----------

This file is taken from the original Bullet engine source, and is MODIFIED to add support for
what we need.

*/
#include <stdio.h>
#include <bullet/LinearMath/btIDebugDraw.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <bullet/LinearMath/btDefaultMotionState.h>
#include "KinematicCharacterController.hpp"
#include "Uncollider.hpp"

namespace glPortal {

static btVector3
getNormalizedVector(const btVector3& v)
{
  btVector3 n(0, 0, 0);

  if (v.length() > SIMD_EPSILON) {
    n = v.normalized();
  }
  return n;
}


///@todo Interact with dynamic objects,
///Ride kinematicly animated platforms properly
///More realistic (or maybe just a config option) falling
/// -> Should integrate falling velocity manually and use that in stepDown()
///Support jumping
///Support ducking
class KinematicClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:
  KinematicClosestNotMeRayResultCallback (btCollisionObject* me) :
    btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
  {
    m_me = me;
  }

  virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,
                                   bool normalInWorldSpace)
  {
    btVector3 p; p.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);
    if (rayResult.m_collisionObject == m_me || Uncollider::isPointInUncollideVolume(p)){
      return 1.0;
    }
    return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
  }
protected:
  btCollisionObject* m_me;
};

class KinematicClosestNotMeConvexResultCallback :
    public btCollisionWorld::ClosestConvexResultCallback
{
public:
  KinematicClosestNotMeConvexResultCallback (btCollisionObject* me,
                                             const btVector3& up, btScalar minSlopeDot)
  : btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0),
                                                  btVector3(0.0, 0.0, 0.0)),
    m_me(me),
    m_up(up),
    m_minSlopeDot(minSlopeDot)
  {
  }

  virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,
                                   bool normalInWorldSpace)
  {
    if (Uncollider::isPointInUncollideVolume(convexResult.m_hitPointLocal)) {
      return 1.0;
    }

    if (convexResult.m_hitCollisionObject == m_me){
      return btScalar(1.0);
    }

    if (not convexResult.m_hitCollisionObject->hasContactResponse()){
      return btScalar(1.0);
    }

    btVector3 hitNormalWorld;
    if (normalInWorldSpace)
    {
      hitNormalWorld = convexResult.m_hitNormalLocal;
    } else
    {
      ///need to transform normal into worldspace
      hitNormalWorld =
        convexResult.m_hitCollisionObject->getWorldTransform().getBasis()*
        convexResult.m_hitNormalLocal;
    }

    btScalar dotUp = m_up.dot(hitNormalWorld);
    if (dotUp < m_minSlopeDot) {
      return btScalar(1.0);
    }

    return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
  }
protected:
  btCollisionObject* m_me;
  const btVector3 m_up;
  btScalar m_minSlopeDot;
};

/*
* Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'
*
* from: http://www-cs-students.stanford.edu/~adityagp/final/node3.html
*/
btVector3 KinematicCharacterController::computeReflectionDirection (const btVector3& direction,
                                                                    const btVector3& normal)
{
  return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

/*
* Returns the portion of 'direction' that is parallel to 'normal'
*/
btVector3 KinematicCharacterController::parallelComponent (const btVector3& direction,
                                                           const btVector3& normal)
{
  btScalar magnitude = direction.dot(normal);
  return normal * magnitude;
}

/*
* Returns the portion of 'direction' that is perpindicular to 'normal'
*/
btVector3 KinematicCharacterController::perpindicularComponent (const btVector3& direction,
                                                                const btVector3& normal)
{
  return direction - parallelComponent(direction, normal);
}

KinematicCharacterController::KinematicCharacterController (btPairCachingGhostObject* ghostObject,
                                                            btConvexShape* convexShape,
                                                            btScalar stepHeight, int upAxis)
{
  m_upAxis = upAxis;
  m_addedMargin = 0.02;
  m_walkDirection.setValue(0, 0, 0);
  m_useGhostObjectSweepTest = true;
  m_ghostObject = ghostObject;
  m_stepHeight = stepHeight;
  m_turnAngle = btScalar(0.0);
  m_convexShape=convexShape;
  m_useWalkDirection = true;// use walk direction by default, legacy behavior
  m_velocityTimeInterval = 0.0;
  m_verticalVelocity = 0.0;
  m_verticalOffset = 0.0;
  m_gravity = 9.8 * 3; // 3G acceleration.
  m_fallSpeed = 55.0; // Terminal velocity of a sky diver in m/s.
  m_jumpSpeed = 10.0;
  m_wasOnGround = false;
  m_wasJumping = false;
  m_interpolateUp = true;
  setMaxSlope(btRadians(45.0));
  m_currentStepOffset = 0;
  full_drop = false;
  bounce_fix = false;
}

KinematicCharacterController::~KinematicCharacterController() {
}

btPairCachingGhostObject* KinematicCharacterController::getGhostObject() {
  return m_ghostObject;
}

bool KinematicCharacterController::recoverFromPenetration(btCollisionWorld *collisionWorld)
{
  // Here we must refresh the overlapping paircache as the penetrating movement itself or the
  // previous recovery iteration might have used setWorldTransform and pushed us into an object
  // that is not in the previous cache contents from the last timestep, as will happen if we
  // are pushed into a new AABB overlap. Unhandled this means the next convex sweep gets stuck.
  //
  // Do this by calling the broadphase's setAabb with the moved AABB, this will update the
  // broadphase paircache and the ghostobject's internal paircache at the same time.   /BW

  btVector3 minAabb, maxAabb;
  m_convexShape->getAabb(m_ghostObject->getWorldTransform(), minAabb, maxAabb);
  collisionWorld->getBroadphase()->setAabb(m_ghostObject->getBroadphaseHandle(),
            minAabb,
            maxAabb,
            collisionWorld->getDispatcher());
  bool penetration = false;

  collisionWorld->getDispatcher()->dispatchAllCollisionPairs(
  m_ghostObject->getOverlappingPairCache(),
  collisionWorld->getDispatchInfo(),
  collisionWorld->getDispatcher()
  );

  m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();

  btScalar maxPen = btScalar(0.0);
  for (int i = 0; i < m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
  {
    m_manifoldArray.resize(0);

    btBroadphasePair* collisionPair =
      &m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

    btCollisionObject* obj0 =
      static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject);
    btCollisionObject* obj1 =
      static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject);

    if ((obj0 && not obj0->hasContactResponse()) || (obj1 && not obj1->hasContactResponse())){
      continue;
    }

    if (collisionPair->m_algorithm){
      collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);
    }

    for (int j=0;j<m_manifoldArray.size();j++)
    {
      btPersistentManifold* manifold = m_manifoldArray[j];
      btScalar directionSign =
        manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
      for (int p=0;p<manifold->getNumContacts();p++)
      {
        const btManifoldPoint&pt = manifold->getContactPoint(p);

        btScalar dist = pt.getDistance();

        if (dist < 0.0)
        {
          if (dist < maxPen)
          {
            maxPen = dist;
            m_touchingNormal = pt.m_normalWorldOnB * directionSign;
          }
          m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
          penetration = true;
        }
      }
    }
  }
  btTransform newTrans = m_ghostObject->getWorldTransform();
  newTrans.setOrigin(m_currentPosition);
  m_ghostObject->setWorldTransform(newTrans);
  return penetration;
}

void KinematicCharacterController::stepUp ( btCollisionWorld* world)
{
  // phase 1: up
  btTransform start, end;
  m_targetPosition =
    m_currentPosition +
    getUpAxisDirections()[m_upAxis] *
    (m_stepHeight + (m_verticalOffset > 0.f?m_verticalOffset:0.f));

  start.setIdentity ();
  end.setIdentity ();

  /* FIXME: Handle penetration properly */
  start.setOrigin (m_currentPosition + getUpAxisDirections()[m_upAxis] *
                   (m_convexShape->getMargin() + m_addedMargin));
  end.setOrigin (m_targetPosition);

  KinematicClosestNotMeConvexResultCallback callback(
                                                     m_ghostObject,
                                                     -getUpAxisDirections()[m_upAxis],
                                                     btScalar(0.7071)
                                                     );
  callback.m_collisionFilterGroup =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
  callback.m_collisionFilterMask =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

  if (m_useGhostObjectSweepTest)
  {
    m_ghostObject->convexSweepTest(m_convexShape, start,
                                   end, callback, world->getDispatchInfo().m_allowedCcdPenetration);
  }
  else
  {
    world->convexSweepTest (m_convexShape, start, end, callback);
  }

  if (callback.hasHit())
  {
    // Only modify the position if the hit was a slope and not a wall or ceiling.
    if (callback.m_hitNormalWorld.dot(getUpAxisDirections()[m_upAxis]) > 0.0)
    {
      // we moved up only a fraction of the step height
      m_currentStepOffset = m_stepHeight * callback.m_closestHitFraction;
      if (m_interpolateUp == true){
        m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition,
                                           callback.m_closestHitFraction);
      } else {
        m_currentPosition = m_targetPosition;
      }
    }
    m_verticalVelocity = 0.0;
    m_verticalOffset = 0.0;
  } else {
    m_currentStepOffset = m_stepHeight;
    m_currentPosition = m_targetPosition;
  }
}

void KinematicCharacterController::updateTargetPositionBasedOnCollision (const btVector3& hitNormal,
                                                                         btScalar tangentMag,
                                                                         btScalar normalMag)
{
  btVector3 movementDirection = m_targetPosition - m_currentPosition;
  btScalar movementLength = movementDirection.length();
  if (movementLength>SIMD_EPSILON)
  {
    movementDirection.normalize();

    btVector3 reflectDir = computeReflectionDirection (movementDirection, hitNormal);
    reflectDir.normalize();

    btVector3 parallelDir, perpindicularDir;

    parallelDir = parallelComponent (reflectDir, hitNormal);
    perpindicularDir = perpindicularComponent (reflectDir, hitNormal);

    m_targetPosition = m_currentPosition;
    if (0)//tangentMag != 0.0)
    {
      btVector3 parComponent = parallelDir * btScalar (tangentMag*movementLength);
      m_targetPosition +=  parComponent;
    }

    if (normalMag != 0.0)
    {
      btVector3 perpComponent = perpindicularDir * btScalar (normalMag*movementLength);
      m_targetPosition += perpComponent;
    }
  }
}

void KinematicCharacterController::stepForwardAndStrafe(btCollisionWorld* collisionWorld,
                                                        const btVector3& walkMove)
{
  // phase 2: forward and strafe
  btTransform start, end;
  m_targetPosition = m_currentPosition + walkMove;

  start.setIdentity ();
  end.setIdentity ();

  btScalar fraction = 1.0;
  btScalar distance2 = (m_currentPosition-m_targetPosition).length2();


  if (m_touchingContact)
  {
    if (m_normalizedDirection.dot(m_touchingNormal) > btScalar(0.0))
    {
      //interferes with step movement
      //updateTargetPositionBasedOnCollision (m_touchingNormal);
    }
  }

  int maxIter = 10;

  while (fraction > btScalar(0.01) && maxIter-- > 0)
  {
    start.setOrigin (m_currentPosition);
    end.setOrigin (m_targetPosition);
    btVector3 sweepDirNegative(m_currentPosition - m_targetPosition);

    KinematicClosestNotMeConvexResultCallback callback(m_ghostObject,
                                                       sweepDirNegative,
                                                       btScalar(0.0));
    callback.m_collisionFilterGroup =
      getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
    callback.m_collisionFilterMask =
      getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;


    btScalar margin = m_convexShape->getMargin();
    m_convexShape->setMargin(margin + m_addedMargin);


    if (m_useGhostObjectSweepTest)
    {
      m_ghostObject->convexSweepTest(m_convexShape,
                                     start, end, callback,
                                     collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
    } else
    {
      collisionWorld->convexSweepTest(m_convexShape, start, end, callback,
                                      collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
    }

    m_convexShape->setMargin(margin);


    fraction -= callback.m_closestHitFraction;

    if (callback.hasHit())
    {
      updateTargetPositionBasedOnCollision (callback.m_hitNormalWorld);
      btVector3 currentDir = m_targetPosition - m_currentPosition;
      distance2 = currentDir.length2();
      if (distance2 > SIMD_EPSILON)
      {
        currentDir.normalize();
        if (currentDir.dot(m_normalizedDirection) <= btScalar(0.0))
        {
          break;
        }
      } else
      {
        break;
      }

    } else {
      m_currentPosition = m_targetPosition;
    }

  }
}

void KinematicCharacterController::stepDown(btCollisionWorld *collisionWorld, btScalar dt) {
  btTransform start, end, end_double;
  bool runonce = false;

  btVector3 orig_position = m_targetPosition;

  btScalar downVelocity = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;

  if (downVelocity > 0.0 and downVelocity > m_fallSpeed
      and (m_wasOnGround or not m_wasJumping)) {
    downVelocity = m_fallSpeed;
  }
  btVector3 step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
  m_targetPosition -= step_drop;

  KinematicClosestNotMeConvexResultCallback callback(m_ghostObject,
    getUpAxisDirections()[m_upAxis],
    m_maxSlopeCosine);
  callback.m_collisionFilterGroup =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
  callback.m_collisionFilterMask =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

  KinematicClosestNotMeConvexResultCallback callback2(m_ghostObject,
    getUpAxisDirections()[m_upAxis],
    m_maxSlopeCosine);
  callback2.m_collisionFilterGroup =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
  callback2.m_collisionFilterMask =
    getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

  while (1) {
    start.setIdentity ();
    end.setIdentity ();

    end_double.setIdentity ();

    start.setOrigin (m_currentPosition);
    end.setOrigin (m_targetPosition);

    //set double test for 2x the step drop, to check for a large drop vs small drop
    end_double.setOrigin (m_targetPosition - step_drop);

    if (m_useGhostObjectSweepTest) {
      m_ghostObject->convexSweepTest(m_convexShape,
                                     start, end, callback,
                                     collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

      if (not callback.hasHit())
      {
        // test a double fall height, to see if the character should interpolate it's fall (full)
        // or not (partial)
        m_ghostObject->convexSweepTest(m_convexShape, start, end_double,
                                       callback2,
                                       collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
      }
    } else {
      collisionWorld->convexSweepTest(m_convexShape, start, end, callback,
                                      collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

      if (not callback.hasHit())
          {
            // test a double fall height, to see if the character should interpolate it's
            // fall (large) or not (small)
            collisionWorld->convexSweepTest(m_convexShape, start, end_double, callback2,
                             collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
          }
    }

    btScalar downVelocity2 = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;
    bool has_hit = false;
    if (bounce_fix == true){
      has_hit = callback.hasHit() || callback2.hasHit();
    } else {
      has_hit = callback2.hasHit();
    }

    if (downVelocity2 > 0.0 && downVelocity2 < m_stepHeight && has_hit == true && runonce == false
          && (m_wasOnGround || not m_wasJumping)) {
      //redo the velocity calculation when falling a small amount, for fast stairs motion
      //for larger falls, use the smoother/slower interpolated movement by not touching
      //the target position

      m_targetPosition = orig_position;
      downVelocity = m_stepHeight;

      btVector3 step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
      m_targetPosition -= step_drop;
      runonce = true;
      continue; //re-run previous tests
    }
    break;
  }

  if (callback.hasHit() or runonce) {
    // we dropped a fraction of the height -> hit floor

    btScalar fraction = (m_currentPosition.getY() - callback.m_hitPointWorld.getY()) / 2;

    if (bounce_fix) {
      if (full_drop) {
        m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition,
          callback.m_closestHitFraction);
      } else {
        //due to errors in the closestHitFraction variable when used with large polygons,
        //calculate the hit fraction manually
        m_currentPosition.setInterpolate3(m_currentPosition,
          m_targetPosition,
          fraction);
      }
    } else {
      m_currentPosition.setInterpolate3 (m_currentPosition, m_targetPosition,
                                         callback.m_closestHitFraction);
    }
    full_drop = false;

    m_verticalVelocity = 0.0;
    m_verticalOffset = 0.0;
    m_wasJumping = false;
  } else {
    // we dropped the full height

    full_drop = true;

    if (bounce_fix) {
      downVelocity = (m_verticalVelocity<0.f?-m_verticalVelocity:0.f) * dt;
      if (downVelocity > m_fallSpeed and (m_wasOnGround or not m_wasJumping)) {
        m_targetPosition += step_drop; //undo previous target change
        downVelocity = m_fallSpeed;
        step_drop = getUpAxisDirections()[m_upAxis] * (m_currentStepOffset + downVelocity);
        m_targetPosition -= step_drop;
      }
    }
    m_currentPosition = m_targetPosition;
  }
}

void KinematicCharacterController::setWalkDirection(const btVector3 &walkDirection) {
  m_useWalkDirection = true;
  m_walkDirection = walkDirection;
  m_normalizedDirection = getNormalizedVector(m_walkDirection);
}

void KinematicCharacterController::setVelocityForTimeInterval(const btVector3 &velocity,
  btScalar timeInterval) {
  m_useWalkDirection = false;
  m_walkDirection = velocity;
  m_normalizedDirection = getNormalizedVector(m_walkDirection);
  m_velocityTimeInterval += timeInterval;
}

void KinematicCharacterController::reset(btCollisionWorld *collisionWorld) {
  m_verticalVelocity = 0.0;
  m_verticalOffset = 0.0;
  m_wasOnGround = false;
  m_wasJumping = false;
  m_walkDirection.setValue(0, 0, 0);
  m_velocityTimeInterval = 0.0;

  //clear pair cache
  btHashedOverlappingPairCache *cache = m_ghostObject->getOverlappingPairCache();
  while (cache->getOverlappingPairArray().size() > 0) {
    cache->removeOverlappingPair(cache->getOverlappingPairArray()[0].m_pProxy0,
      cache->getOverlappingPairArray()[0].m_pProxy1,
      collisionWorld->getDispatcher());
  }
}

void KinematicCharacterController::warp(const btVector3& origin) {
  btTransform xform;
  xform.setIdentity();
  xform.setOrigin (origin);
  m_ghostObject->setWorldTransform(xform);
}


void KinematicCharacterController::preStep(btCollisionWorld *collisionWorld) {
  int numPenetrationLoops = 0;
  m_touchingContact = false;
  while (recoverFromPenetration(collisionWorld)) {
    numPenetrationLoops++;
    m_touchingContact = true;
    if (numPenetrationLoops > 4) {
      break;
    }
  }

  m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();
  m_targetPosition = m_currentPosition;
}

void KinematicCharacterController::playerStep(btCollisionWorld *collisionWorld, btScalar dt) {
  // quick check...
  if (not m_useWalkDirection and (m_velocityTimeInterval <= 0.0 or m_walkDirection.fuzzyZero())) {
    return;
  }

  m_wasOnGround = onGround();

  // Update fall velocity.
  m_verticalVelocity -= m_gravity * dt;
  if (m_verticalVelocity > 0.0 && m_verticalVelocity > m_jumpSpeed) {
    m_verticalVelocity = m_jumpSpeed;
  }
  if (m_verticalVelocity < 0.0 && btFabs(m_verticalVelocity) > btFabs(m_fallSpeed)) {
    m_verticalVelocity = -btFabs(m_fallSpeed);
  }
  m_verticalOffset = m_verticalVelocity * dt;


  btTransform xform;
  xform = m_ghostObject->getWorldTransform ();

  stepUp (collisionWorld);
  if (m_useWalkDirection) {
    stepForwardAndStrafe (collisionWorld, m_walkDirection);
  } else {
    // still have some time left for moving!
    btScalar dtMoving =
      (dt < m_velocityTimeInterval) ? dt : m_velocityTimeInterval;
    m_velocityTimeInterval -= dt;

    // how far will we move while we are moving?
    btVector3 move = m_walkDirection * dtMoving;

    // okay, step
    stepForwardAndStrafe(collisionWorld, move);
  }
  stepDown (collisionWorld, dt);

  xform.setOrigin (m_currentPosition);
  m_ghostObject->setWorldTransform (xform);
}

void KinematicCharacterController::setFallSpeed(btScalar fallSpeed) {
  m_fallSpeed = fallSpeed;
}

void KinematicCharacterController::setJumpSpeed(btScalar jumpSpeed) {
  m_jumpSpeed = jumpSpeed;
}

void KinematicCharacterController::setMaxJumpHeight(btScalar maxJumpHeight) {
  m_maxJumpHeight = maxJumpHeight;
}

bool KinematicCharacterController::canJump() const {
  return onGround();
}

void KinematicCharacterController::jump() {
  if (not canJump()) {
    return;
  }

  m_verticalVelocity = m_jumpSpeed;
  m_wasJumping = true;
}

void KinematicCharacterController::setGravity(btScalar gravity) {
  m_gravity = gravity;
}

btScalar KinematicCharacterController::getGravity() const {
  return m_gravity;
}

void KinematicCharacterController::setMaxSlope(btScalar slopeRadians)
{
  m_maxSlopeRadians = slopeRadians;
  m_maxSlopeCosine = btCos(slopeRadians);
}

btScalar KinematicCharacterController::getMaxSlope() const
{
  return m_maxSlopeRadians;
}

bool KinematicCharacterController::onGround () const
{
  return m_verticalVelocity == 0.0 && m_verticalOffset == 0.0;
}


btVector3* KinematicCharacterController::getUpAxisDirections()
{
  static btVector3 sUpAxisDirection[3] =
    { btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f) };

  return sUpAxisDirection;
}

void KinematicCharacterController::debugDraw(btIDebugDraw* debugDrawer)
{
}

void KinematicCharacterController::setUpInterpolate(bool value)
{
  m_interpolateUp = value;
}

} /* namespace glPortal */
