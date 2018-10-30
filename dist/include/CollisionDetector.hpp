#ifndef COLLISION_DETECTOR_HPP
#define COLLISION_DETECTOR_HPP

#include <OrientedBoundingBox.hpp>
#include <Contact.hpp>

class CollisionDetector {
public:
    static bool isVolumeIntersectingVolume(
        BoundingVolume* boundingVolume1,
        BoundingVolume* boundingVolume2
    ) noexcept;

    static bool isSphereIntersectingVolume(
        BoundingSphere* sphere,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isCapsuleIntersectingVolume(
        BoundingCapsule* capsule,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isOBBIntersectingVolume(
        OrientedBoundingBox* obb,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isSphereIntersectingSphere(
        BoundingSphere* sphere1,
        BoundingSphere* sphere2
    ) noexcept;

    static bool isCapsuleIntersectingSphere(
        BoundingCapsule* capsule,
        BoundingSphere* sphere
    ) noexcept;

    static bool isCapsuleIntersectingCapsule(
        BoundingCapsule* capsule1,
        BoundingCapsule* capsule2
    ) noexcept;

    static bool isOBBIntersectingSphere(
        OrientedBoundingBox* obb,
        BoundingSphere* sphere
    ) noexcept;

    static bool isOBBIntersectingCapsule(
        OrientedBoundingBox* obb,
        BoundingCapsule* capsule
    ) noexcept;

    static bool isOBBIntersectingOBB(
        OrientedBoundingBox* obb1,
        OrientedBoundingBox* obb2
    ) noexcept;

    static bool isVolumeEnclosingVolume(
        BoundingVolume* boundingVolume1,
        BoundingVolume* boundingVolume2
    ) noexcept;

    static bool isSphereEnclosingVolume(
        BoundingSphere* sphere,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isCapsuleEnclosingVolume(
        BoundingCapsule* capsule,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isOBBEnclosingVolume(
        OrientedBoundingBox* obb,
        BoundingVolume* boundingVolume
    ) noexcept;

    static bool isSphereEnclosingSphere(
        BoundingSphere* sphere1,
        BoundingSphere* sphere2
    ) noexcept;

    static bool isSphereEnclosingCapsule(
        BoundingSphere* sphere,
        BoundingCapsule* capsule
    ) noexcept;

    static bool isSphereEnclosingOBB(
        BoundingSphere* sphere,
        OrientedBoundingBox* obb
    ) noexcept;

    static bool isCapsuleEnclosingSphere(
        BoundingCapsule* capsule,
        BoundingSphere* sphere
    ) noexcept;

    static bool isCapsuleEnclosingCapsule(
        BoundingCapsule* capsule1,
        BoundingCapsule* capsule2
    ) noexcept;

    static bool isCapsuleEnclosingOBB(
        BoundingCapsule* capsule,
        OrientedBoundingBox* obb
    ) noexcept;

    static bool isOBBEnclosingSphere(
        OrientedBoundingBox* obb,
        BoundingSphere* sphere
    ) noexcept;

    static bool isOBBEnclosingCapsule(
        OrientedBoundingBox* obb,
        BoundingCapsule* capsule
    ) noexcept;

    static bool isOBBEnclosingOBB(
        OrientedBoundingBox* obb1,
        OrientedBoundingBox* obb2
    ) noexcept;

    static Contact findContactBetweenVolumeAndVolume(
        BoundingVolume* boundingVolume1,
        BoundingVolume* boundingVolume2
    ) noexcept;

    static Contact findContactBetweenSphereAndVolume(
        BoundingSphere* sphere,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact findContactBetweenCapsuleAndVolume(
        BoundingCapsule* capsule,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact findContactBetweenOBBAndVolume(
        OrientedBoundingBox* obb,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact findContactBetweenSphereAndSphere(
        BoundingSphere* sphere1,
        BoundingSphere* sphere2
    ) noexcept;

    static Contact findContactBetweenSphereAndCapsule(
        BoundingSphere* sphere,
        BoundingCapsule* capsule
    ) noexcept;

    static Contact findContactBetweenSphereAndOBB(
        BoundingSphere* sphere,
        OrientedBoundingBox* obb
    ) noexcept;

    static Contact findContactBetweenCapsuleAndSphere(
        BoundingCapsule* capsule,
        BoundingSphere* sphere
    ) noexcept;

    static Contact findContactBetweenCapsuleAndCapsule(
        BoundingCapsule* capsule1,
        BoundingCapsule* capsule2
    ) noexcept;

    static Contact findContactBetweenCapsuleAndOBB(
        BoundingCapsule* capsule,
        OrientedBoundingBox* obb
    ) noexcept;

    static Contact findContactBetweenOBBAndSphere(
        OrientedBoundingBox* obb,
        BoundingSphere* sphere
    ) noexcept;

    static Contact findContactBetweenOBBAndCapsule(
        OrientedBoundingBox* obb,
        BoundingCapsule* capsule
    ) noexcept;

    static Contact findContactBetweenOBBAndOBB(
        OrientedBoundingBox* obb1,
        OrientedBoundingBox* obb2
    ) noexcept;
};

#endif // !COLLISION_DETECTOR_HPP
