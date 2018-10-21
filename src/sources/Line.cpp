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
    return GeometryUtils::getClosestPointToTestPointAndSegment(pt, start, end);
}

vec3 Line::getClosestPtPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept {
    return GeometryUtils::getClosestPointToTestPointAndSegment(pt, start, end);
}

GLfloat Line::getSquaredDistPointSegment(const vec3& pt) const noexcept {
    return GeometryUtils::getSquaredDistToTestPointAndSegment(pt, start, end);
}

GLfloat Line::getSquaredDistPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept {
    return GeometryUtils::getSquaredDistToTestPointAndSegment(pt, start, end);
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line2) const noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        start,
        end,
        line2.getPointStart(),
        line2.getPointEnd()
    );
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start2, const vec3& end2) const noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        start,
        end,
        start2,
        end2
    );
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const Line& line2) noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        line1.getPointStart(),
        line1.getPointEnd(),
        line2.getPointStart(),
        line2.getPointEnd()
    );
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const Line& line2) noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        start1,
        end1,
        line2.getPointStart(),
        line2.getPointEnd()
    );
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const vec3& start2, const vec3& end2) noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        line1.getPointStart(),
        line1.getPointEnd(),
        start2,
        end2
    );
}

GLfloat Line::getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const vec3& start2, const vec3& end2) noexcept {
    return GeometryUtils::getClosestPointsBetweenSegments(
        c1,
        c2,
        start1,
        end1,
        start2,
        end2
    );
}