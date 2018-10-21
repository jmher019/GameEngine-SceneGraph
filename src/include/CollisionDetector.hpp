#ifndef COLLISION_DETECTOR_HPP
#define COLLISION_DETECTOR_HPP

#include <OrientedBoundingBox.hpp>
#include <Contact.hpp>

class CollisionDetector {
public:
    static Contact isVolumeIntersectingVolume(
        BoundingVolume* boundingVolume1,
        BoundingVolume* boundingVolume2
    ) noexcept;

    static Contact isSphereIntersectingVolume(
        BoundingSphere* sphere,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact isCapsuleIntersectingVolume(
        BoundingCapsule* capsule,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact isOBBIntersectingVolume(
        OrientedBoundingBox* obb,
        BoundingVolume* boundingVolume
    ) noexcept;

    static Contact isSphereIntersectingSphere(
        BoundingSphere* sphere1,
        BoundingSphere* sphere2
    ) noexcept;

    static Contact isCapsuleIntersectingSphere(
        BoundingCapsule* capsule,
        BoundingSphere* sphere,
        GLboolean reverseContactTarget
    ) noexcept;

    static Contact isCapsuleIntersectingCapsule(
        BoundingCapsule* capsule1,
        BoundingCapsule* capsule2
    ) noexcept;

    static Contact isOBBIntersectingSphere(
        OrientedBoundingBox* obb,
        BoundingSphere* sphere,
        GLboolean reverseContactTarget
    ) noexcept;

    static Contact isOBBIntersectingCapsule(
        OrientedBoundingBox* obb,
        BoundingCapsule* capsule,
        GLboolean reverseContactTarget
    ) noexcept;

    static Contact isOBBIntersectingOBB(
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
};

#endif // !COLLISION_DETECTOR_HPP
