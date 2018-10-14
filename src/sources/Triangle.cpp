#include <Triangle.hpp>

Triangle::Triangle(const vec3& v1, const vec3& v2, const vec3& v3):
    v1(v1),
    v2(v2),
    v3(v2) {
}

Triangle::Triangle(const Triangle& triangle):
    v1(triangle.v1),
    v2(triangle.v2), 
    v3(triangle.v3) {
}

Triangle::Triangle(Triangle&& triangle):
    v1(move(triangle.v1)),
    v2(move(triangle.v2)),
    v3(move(triangle.v3)) {
}

Triangle& Triangle::operator=(const Triangle& triangle) noexcept {
    v1 = triangle.v1;
    v2 = triangle.v2;
    v3 = triangle.v3;

    return *this;
}

Triangle& Triangle::operator=(Triangle&& triangle) noexcept {
    v1 = move(triangle.v1);
    v2 = move(triangle.v2);
    v3 = move(triangle.v3);

    return *this;
}

const vec3& Triangle::getVertex1(void) const noexcept {
    return v1;
}

void Triangle::setVertex1(const vec3& v1) noexcept {
    this->v1 = v1;
}

const vec3& Triangle::getVertex2(void) const noexcept {
    return v2;
}

void Triangle::setVertex2(const vec3& v2) noexcept {
    this->v2 = v2;
}

const vec3& Triangle::getVertex3(void) const noexcept {
    return v3;
}

void Triangle::setVertex3(const vec3& v3) noexcept {
    this->v3 = v3;
}

vec3 Triangle::getNormal(void) const noexcept {
    return cross(v2 - v1, v3 - v2);
}

vec3 Triangle::getNormal(const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    return cross(v2 - v1, v3 - v2);
}

vec3 Triangle::getBarycentricCoord(const vec3& p) const noexcept {
    const vec3 vec0 = v2 - v1, vec1 = v3 - v1, vec2 = p - v1;
    const GLfloat d00 = dot(vec0, vec0);
    const GLfloat d01 = dot(vec0, vec1);
    const GLfloat d11 = dot(vec1, vec1);
    const GLfloat d20 = dot(vec2, vec0);
    const GLfloat d21 = dot(vec2, vec1);
    const GLfloat denom = d00 * d11 - d01 * d01;

    const GLfloat v = (d11 * d20 - d01 * d21) / denom;
    const GLfloat w = (d00 * d21 - d01 * d20) / denom;
    return vec3(1.0f - v - w, v, w);
}

vec3 Triangle::getBarycentricCoord(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    const vec3 vec0 = v2 - v1, vec1 = v3 - v1, vec2 = p - v1;
    const GLfloat d00 = dot(vec0, vec0);
    const GLfloat d01 = dot(vec0, vec1);
    const GLfloat d11 = dot(vec1, vec1);
    const GLfloat d20 = dot(vec2, vec0);
    const GLfloat d21 = dot(vec2, vec1);
    const GLfloat denom = d00 * d11 - d01 * d01;

    const GLfloat v = (d11 * d20 - d01 * d21) / denom;
    const GLfloat w = (d00 * d21 - d01 * d20) / denom;
    return vec3(1.0f - v - w, v, w);
}

bool Triangle::isWithinTriangle(const vec3& p) const noexcept {
    const vec3 barycentricCoord = getBarycentricCoord(p);

    return barycentricCoord[1] >= 0.0f &&
        barycentricCoord[2] >= 0.0f &&
        (barycentricCoord[1] + barycentricCoord[2]) <= 1.0f;
}

bool Triangle::isWithinTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    const vec3 barycentricCoord = getBarycentricCoord(p, v1, v2, v3);

    return barycentricCoord[1] >= 0.0f &&
        barycentricCoord[2] >= 0.0f &&
        (barycentricCoord[1] + barycentricCoord[2]) <= 1.0f;
}

vec3 Triangle::getClosestPtPointTriangle(const vec3& p) const noexcept {
    // Check if P in vertex region outside A
    const vec3 ab = v2 - v1;
    const vec3 ac = v3 - v1;
    const vec3 ap = p - v1;
    const GLfloat d1 = dot(ab, ap);
    const GLfloat d2 = dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return v1; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    const vec3 bp = p - v2;
    const GLfloat d3 = dot(ab, bp);
    const GLfloat d4 = dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return v2; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    const GLfloat vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        const GLfloat v = d1 / (d1 - d3);
        return v1 + v * ab; // barycentric coordinates (1-v,v,0)
    }
    // Check if P in vertex region outside C
    const vec3 cp = p - v3;
    const GLfloat d5 = dot(ab, cp);
    const GLfloat d6 = dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return v3; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    const GLfloat vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        const GLfloat w = d2 / (d2 - d6);
        return v1 + w * ac; // barycentric coordinates (1-w,0,w)
    }
    // Check if P in edge region of BC, if so return projection of P onto BC
    const GLfloat va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        const GLfloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return v2 + w * (v3 - v2); // barycentric coordinates (0,1-w,w)
    }
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    const GLfloat denom = 1.0f / (va + vb + vc);
    const GLfloat v = vb * denom;
    const GLfloat w = vc * denom;
    return v1 + ab * v + ac * w; // = u*v1 + v*b + w*c, u = va * denom = 1.0f-v-w
}

vec3 Triangle::getClosestPtPointTriangle(const vec3& p, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    // Check if P in vertex region outside A
    const vec3 ab = v2 - v1;
    const vec3 ac = v3 - v1;
    const vec3 ap = p - v1;
    const GLfloat d1 = dot(ab, ap);
    const GLfloat d2 = dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return v1; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    const vec3 bp = p - v2;
    const GLfloat d3 = dot(ab, bp);
    const GLfloat d4 = dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return v2; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    const GLfloat vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        const GLfloat v = d1 / (d1 - d3);
        return v1 + v * ab; // barycentric coordinates (1-v,v,0)
    }
    // Check if P in vertex region outside C
    const vec3 cp = p - v3;
    const GLfloat d5 = dot(ab, cp);
    const GLfloat d6 = dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return v3; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    const GLfloat vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        const GLfloat w = d2 / (d2 - d6);
        return v1 + w * ac; // barycentric coordinates (1-w,0,w)
    }
    // Check if P in edge region of BC, if so return projection of P onto BC
    const GLfloat va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        const GLfloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return v2 + w * (v3 - v2); // barycentric coordinates (0,1-w,w)
    }
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    const GLfloat denom = 1.0f / (va + vb + vc);
    const GLfloat v = vb * denom;
    const GLfloat w = vc * denom;
    return v1 + ab * v + ac * w; // = u*v1 + v*b + w*c, u = va * denom = 1.0f-v-w
}

bool Triangle::isSegmentIntersecting(vec3& c, const Line& line) const noexcept {
    const vec3 n = getNormal();
    const GLfloat d = dot(v1, n);

    const vec3 lVec = line.getPointEnd() - line.getPointStart();
    const GLfloat len2 = dot(lVec, lVec);

    // if the line degenerates to a point
    if (len2 < GeometryUtils::epsilon) {
        c = line.getPointStart();
        return isWithinTriangle(c);
    }

    const GLfloat t = dot(v1 - line.getPointStart(), n) / dot(lVec, n);

    if (t < 0.f || t > 1.f) {
        return false;
    }

    c = t * lVec + line.getPointStart();
    return isWithinTriangle(c);
}

bool Triangle::isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end) const noexcept {
    const vec3 n = getNormal();
    const GLfloat d = dot(v1, n);

    const vec3 lVec = end - start;
    const GLfloat len2 = dot(lVec, lVec);

    // if the line degenerates to a point
    if (len2 < GeometryUtils::epsilon) {
        c = start;
        return isWithinTriangle(c);
    }

    const GLfloat t = dot(v1 - start, n) / dot(lVec, n);

    if (t < 0.f || t > 1.f) {
        return false;
    }

    c = t * lVec + start;
    return isWithinTriangle(c);
}

bool Triangle::isSegmentIntersecting(vec3& c, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    const vec3 n = getNormal(v1, v2, v3);
    const GLfloat d = dot(v1, n);

    const vec3 lVec = line.getPointEnd() - line.getPointStart();
    const GLfloat len2 = dot(lVec, lVec);

    // if the line degenerates to a point
    if (len2 < GeometryUtils::epsilon) {
        c = line.getPointStart();
        return isWithinTriangle(c, v1, v2, v3);
    }

    const GLfloat t = dot(v1 - line.getPointStart(), n) / dot(lVec, n);

    if (t < 0.f || t > 1.f) {
        return false;
    }

    c = t * lVec + line.getPointStart();
    return isWithinTriangle(c, v1, v2, v3);
}

bool Triangle::isSegmentIntersecting(vec3& c, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    const vec3 n = getNormal(v1, v2, v3);
    const GLfloat d = dot(v1, n);

    const vec3 lVec = end - start;
    const GLfloat len2 = dot(lVec, lVec);

    // if the line degenerates to a point
    if (len2 < GeometryUtils::epsilon) {
        c = start;
        return isWithinTriangle(c, v1, v2, v3);
    }

    const GLfloat t = dot(v1 - start, n) / dot(lVec, n);

    if (t < 0.f || t > 1.f) {
        return false;
    }

    c = t * lVec + start;
    return isWithinTriangle(c, v1, v2, v3);
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line) const noexcept {
    if (isSegmentIntersecting(c1, line)) {
        c2 = c1;
        return 0.f;
    }

    vector<pair<vec3, vec3>> pairs;
    // closest point between l and v1 -> v2 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v1, v2);

    // closest point between l and v2 -> v3 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v2, v3);

    // closest point between l and v3 -> v1 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v3, v1);

    // closest point from the first vertex in l to the plane containing the triangle
    const vec3 n = getNormal();
    const GLfloat d = dot(v1, n);
    const vec3 pc = Plane::closestPtPointPlane(line.getPointStart(), n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(pc)) {
        pairs.push_back(make_pair(line.getPointStart(), pc));
    }

    // closest point from the second vertex in l to the plane containing the triangle
    const vec3 qc = Plane::closestPtPointPlane(line.getPointEnd(), n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(qc)) {
        pairs.push_back(make_pair(line.getPointEnd(), qc));
    }

    pair<vec3, vec3>& result = pairs[0];
    vec3 diff = result.second - result.first;
    GLfloat dist2 = dot(diff, diff);

    for (size_t i = 1; i < pairs.size(); i++) {
        diff = pairs[i].second - pairs[i].first;
        float currDist2 = dot(diff, diff);
        if (dist2 > currDist2) {
            dist2 = currDist2;
            result = pairs[i];
        }
    }

    c1 = result.first;
    c2 = result.second;
    return dist2;
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end) const noexcept {
    if (isSegmentIntersecting(c1, start, end)) {
        c2 = c1;
        return 0.f;
    }

    vector<pair<vec3, vec3>> pairs;
    // closest point between l and v1 -> v2 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v1, v2);

    // closest point between l and v2 -> v3 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v2, v3);

    // closest point between l and v3 -> v1 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v3, v1);

    // closest point from the first vertex in l to the plane containing the triangle
    const vec3 n = getNormal();
    const float d = dot(v1, n);
    const vec3 pc = Plane::closestPtPointPlane(start, n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(pc)) {
        pairs.push_back(make_pair(start, pc));
    }

    // closest point from the second vertex in l to the plane containing the triangle
    const vec3 qc = Plane::closestPtPointPlane(end, n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(qc)) {
        pairs.push_back(make_pair(end, qc));
    }

    pair<vec3, vec3>& result = pairs[0];
    vec3 diff = result.second - result.first;
    float dist2 = dot(diff, diff);

    for (size_t i = 1; i < pairs.size(); i++) {
        diff = pairs[i].second - pairs[i].first;
        float currDist2 = dot(diff, diff);
        if (dist2 > currDist2) {
            dist2 = currDist2;
            result = pairs[i];
        }
    }

    c1 = result.first;
    c2 = result.second;
    return dist2;
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const Line& line, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    if (isSegmentIntersecting(c1, line, v1, v2, v3)) {
        c2 = c1;
        return 0.f;
    }

    vector<pair<vec3, vec3>> pairs;
    // closest point between l and v1 -> v2 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v1, v2);

    // closest point between l and v2 -> v3 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v2, v3);

    // closest point between l and v3 -> v1 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    line.getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, v3, v1);

    // closest point from the first vertex in l to the plane containing the triangle
    const vec3 n = getNormal(v1, v2, v3);
    const float d = dot(v1, n);
    const vec3 pc = Plane::closestPtPointPlane(line.getPointStart(), n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(pc, v1, v2, v3)) {
        pairs.push_back(make_pair(line.getPointStart(), pc));
    }

    // closest point from the second vertex in l to the plane containing the triangle
    const vec3 qc = Plane::closestPtPointPlane(line.getPointEnd(), n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(qc, v1, v2, v3)) {
        pairs.push_back(make_pair(line.getPointEnd(), qc));
    }

    pair<vec3, vec3>& result = pairs[0];
    vec3 diff = result.second - result.first;
    float dist2 = dot(diff, diff);

    for (size_t i = 1; i < pairs.size(); i++) {
        diff = pairs[i].second - pairs[i].first;
        float currDist2 = dot(diff, diff);
        if (dist2 > currDist2) {
            dist2 = currDist2;
            result = pairs[i];
        }
    }

    c1 = result.first;
    c2 = result.second;
    return dist2;
}

GLfloat Triangle::getClosestPtSegmentTriangle(vec3& c1, vec3& c2, const vec3& start, const vec3& end, const vec3& v1, const vec3& v2, const vec3& v3) noexcept {
    if (isSegmentIntersecting(c1, start, end, v1, v2, v3)) {
        c2 = c1;
        return 0.f;
    }

    vector<pair<vec3, vec3>> pairs;
    // closest point between l and v1 -> v2 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v1, v2);

    // closest point between l and v2 -> v3 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v2, v3);

    // closest point between l and v3 -> v1 segment
    pairs.push_back(make_pair(vec3(), vec3()));
    Line::getClosestPtSegmentSegment(pairs.back().second, pairs.back().first, start, end, v3, v1);

    // closest point from the first vertex in l to the plane containing the triangle
    const vec3 n = getNormal(v1, v2, v3);
    const float d = dot(v1, n);
    const vec3 pc = Plane::closestPtPointPlane(start, n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(pc, v1, v2, v3)) {
        pairs.push_back(make_pair(start, pc));
    }

    // closest point from the second vertex in l to the plane containing the triangle
    const vec3 qc = Plane::closestPtPointPlane(end, n, d);
    // check if that point is within the triangle
    if (isWithinTriangle(qc, v1, v2, v3)) {
        pairs.push_back(make_pair(end, qc));
    }

    pair<vec3, vec3>& result = pairs[0];
    vec3 diff = result.second - result.first;
    float dist2 = dot(diff, diff);

    for (size_t i = 1; i < pairs.size(); i++) {
        diff = pairs[i].second - pairs[i].first;
        float currDist2 = dot(diff, diff);
        if (dist2 > currDist2) {
            dist2 = currDist2;
            result = pairs[i];
        }
    }

    c1 = result.first;
    c2 = result.second;
    return dist2;
}