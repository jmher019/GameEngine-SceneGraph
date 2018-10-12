#ifndef LINE_HPP
#define LINE_HPP

#include <Vertex.hpp>

class Line {
private:
    vec3 start;
    vec3 end;

public:
    Line(const vec3& start, const vec3& end);

    Line(const Line& line);

    Line(Line&& line);

    Line& operator=(const Line& line) noexcept;

    Line& operator=(Line&& line) noexcept;

    const vec3& getPointStart(void) const noexcept;

    void setPointStart(const vec3& start) noexcept;

    const vec3& getPointEnd(void) const noexcept;

    void setPointEnd(const vec3& end) noexcept;

    vec3 getClosestPtPointSegment(const vec3& pt) const noexcept;
    
    static vec3 getClosestPtPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept;

    GLfloat getSquaredDistPointSegment(const vec3& pt) const noexcept;
    
    static GLfloat getSquaredDistPointSegment(const vec3& pt, const vec3& start, const vec3& end) noexcept;

    GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line2) const noexcept;
    
    GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start2, const vec3& end2) const noexcept;
    
    static GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const Line& line2) noexcept;
    
    static GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const Line& line2) noexcept;
    
    static GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const Line& line1, const vec3& start2, const vec3& end2) noexcept;
    
    static GLfloat getClosestPtSegmentSegment(vec3& c1, vec3& c2, const vec3& start1, const vec3& end1, const vec3& start2, const vec3& end2) noexcept;
};

#endif // !LINE_HPP
