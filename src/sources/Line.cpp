#include <Line.hpp>

Line::Line(const vec3& start, const vec3& end):
    start(start),
    end(end) {
}

Line::Line(const Line& line):
    start(line.start),
    end(line.end) {
}

Line::Line(Line&& line):
    start(move(line.start)),
    end(move(line.end)){
}

Line& Line::operator=(const Line& line) noexcept {
    start = line.start;
    end = line.end;

    return *this;
}

Line& Line::operator=(Line&& line) noexcept {
    start = move(line.start);
    end = move(line.end);

    return *this;
}

const vec3& Line::getPointStart(void) const noexcept {
    return start;
}

void Line::setPointStart(const vec3& start) noexcept {
    this->start = start;
}

const vec3& Line::getPointEnd(void) const noexcept {
    return end;
}

void Line::setPointEnd(const vec3& end) noexcept {
    this->end = end;
}

vec3 Line::getClosestPtPointSegment(const vec3& pt) const noexcept {
    const vec3 ab = end - start;
    // Project p onto ab, but deferring divide by Dot(ab, ab)
    const GLfloat t = dot(pt - start, ab);
    if (t <= 0.0f) {
        // p projects outside the [start, end] interval, on the start side; clamp to start
        return start;
    }

    const GLfloat denom = dot(ab, ab); // Always nonnegative since denom = ||ab||^2
    if (t >= denom) {
        // p projects outside the [start, end] interval, on the end side; clamp to end
        return end;
    }

    return start + ab * t / denom;
}

vec3 Line::getClosestPtPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept {
    const vec3 ab = end - start;
    // Project p onto ab, but deferring divide by Dot(ab, ab)
    const GLfloat t = dot(pt - start, ab);
    if (t <= 0.0f) {
        // p projects outside the [start, end] interval, on the start side; clamp to start
        return vec3(start);
    }

    const GLfloat denom = dot(ab, ab); // Always nonnegative since denom = ||ab||^2
    if (t >= denom) {
        // p projects outside the [start, end] interval, on the end side; clamp to end
        return vec3(end);
    }

    return start + ab * t / denom;
}

GLfloat Line::getSquaredDistPointSegment(const vec3& pt) const noexcept {
    const vec3 ab = end - start, ap = pt - start, bp = pt - end;
    const float e = dot(ap, ab);

    // Handle cases where c projects outside ab
    if (e <= 0.0f) return dot(ap, ap);

    const float f = dot(ab, ab);
    if (e >= f) return dot(bp, bp);

    // Handle cases where p projects onto ab
    return dot(ap, ap) - e * e / f;
}

GLfloat Line::getSquaredDistPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept {
    const vec3 ab = end - start, ap = pt - start, bp = pt - end;
    const float e = dot(ap, ab);

    // Handle cases where c projects outside ab
    if (e <= 0.0f) return dot(ap, ap);

    const float f = dot(ab, ab);
    if (e >= f) return dot(bp, bp);

    // Handle cases where p projects onto ab
    return dot(ap, ap) - e * e / f;
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line2) const noexcept {
    const vec3 d1 = end - start;
    const vec3 d2 = line2.end - line2.start;
    const vec3 r = start - line2.start;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = vec3(start);
        c2 = vec3(line2.start);
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.0f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = start + d1 * s;
    c2 = line2.start + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start2, const vec3& end2) const noexcept {
    const vec3 d1 = end - start;
    const vec3 d2 = end2 - start2;
    const vec3 r = start - start2;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = vec3(start);
        c2 = vec3(start2);
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = start + d1 * s;
    c2 = start2 + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const Line& line2) noexcept {
    const vec3 d1 = line1.end - line1.start;
    const vec3 d2 = line2.end - line2.start;
    const vec3 r = line1.start - line2.start;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = vec3(line1.start);
        c2 = vec3(line2.start);
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = line1.start + d1 * s;
    c2 = line2.start + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const Line& line2) noexcept {
    const vec3 d1 = end1 - start1;
    const vec3 d2 = line2.end - line2.start;
    const vec3 r = start1 - line2.start;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = vec3(start1);
        c2 = vec3(line2.start);
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = start1 + d1 * s;
    c2 = line2.start + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const vec3& start2, const vec3& end2) noexcept {
    const vec3 d1 = line1.end - line1.start;
    const vec3 d2 = end2 - start2;
    const vec3 r = line1.start - start2;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = vec3(line1.start);
        c2 = vec3(start2);
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = line1.start + d1 * s;
    c2 = start2 + d2 * t;
    return dot(c2 - c1, c2 - c1);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const vec3& start2, const vec3& end2) noexcept {
    const vec3 d1 = end1 - start1;
    const vec3 d2 = end2 - start2;
    const vec3 r = start1 - start2;
    const float b = dot(d1, d1);
    const float e = dot(d2, d2);
    const float f = dot(d2, r);
    float s = 0.f, t = 0.f;

    // Check if either or both segments degenerate into points
    if (b <= GeometryUtils::epsilon && e <= GeometryUtils::epsilon) {
        // Both segments degenerate into points
        c1 = start1;
        c2 = start2;
        return dot(c2 - c1, c2 - c1);
    }

    if (b <= GeometryUtils::epsilon) {
        // First segment degenerates into a point
        t = clamp(f / e, 0.f, 1.f);
    }
    else {
        const float c = dot(d1, r);
        if (e <= GeometryUtils::epsilon) {
            // second segment degenerates to a point
            s = clamp(-c / b, 0.f, 1.f);
        }
        else {
            // The general nondegenerate case starts here
            const float d = dot(d1, d2);
            const float denom = b * e - d * d;

            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment s1. Else pick arbitrary s (here 0)
            if (denom != 0.f) {
                s = clamp((d * f - c * e) / denom, 0.f, 1.f);
            }
            else {
                s = 0.f;
            }

            t = (d * s + f) / e;

            // If t in [0, 1] done. Else clamp t, recompute s for the new value of t
            if (t < 0.f) {
                t = 0.f;
                s = clamp(-c / b, 0.f, 1.f);
            }
            else if (t > 1.f) {
                t = 1.f;
                s = clamp((d - c) / b, 0.f, 1.f);
            }
        }
    }

    c1 = start1 + d1 * s;
    c2 = start2 + d2 * t;
    return dot(c2 - c1, c2 - c1);
}