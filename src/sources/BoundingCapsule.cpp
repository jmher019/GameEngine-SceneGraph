#include <BoundingCapsule.hpp>

BoundingCapsule::BoundingCapsule(const GLfloat& capsuleLineLength, const GLfloat& radius, const string& name, const Transform& transform):
    capsuleLineLength(capsuleLineLength),
    radius(radius),
    BoundingVolume(name, transform) {
    vertices = getVerticesForGrid();
    lineVertices = getVerticesForLine();
    initialize();
}

BoundingCapsule::BoundingCapsule(const BoundingCapsule& capsule):
    capsuleLineLength(capsule.capsuleLineLength),
    radius(capsule.radius),
    BoundingVolume(name, transform) {
    vertices = capsule.vertices;
    lineVertices = capsule.lineVertices;
    children = capsule.children;
    initialize();
}

BoundingCapsule::BoundingCapsule(BoundingCapsule&& capsule) {
    name = move(capsule.name);
    transform = move(capsule.transform);
    children = move(capsule.children);
    VAO = move(capsule.VAO);
    VBO = move(capsule.VBO);
    lineVAO = move(capsule.lineVAO);
    lineVBO = move(capsule.lineVBO);
    capsuleLineLength = move(capsule.capsuleLineLength);
    radius = move(capsule.radius);
    lineVertices = move(capsule.lineVertices);
    vertices = move(capsule.vertices);
}

BoundingCapsule& BoundingCapsule::operator=(const BoundingCapsule& capsule) noexcept {
    name = capsule.name;
    transform = capsule.transform;
    children = capsule.children;
    capsuleLineLength = capsule.capsuleLineLength;
    radius = capsule.radius;

    return *this;
}

BoundingCapsule& BoundingCapsule::operator=(BoundingCapsule&& capsule) noexcept {
    deallocate();

    name = move(capsule.name);
    transform = move(capsule.transform);
    children = move(capsule.children);
    VAO = move(capsule.VAO);
    VBO = move(capsule.VBO);
    lineVAO = move(capsule.lineVAO);
    lineVBO = move(capsule.lineVBO);
    capsuleLineLength = move(capsule.capsuleLineLength);
    radius = move(capsule.radius);
    lineVertices = move(capsule.lineVertices);
    vertices = move(capsule.vertices);

    return *this;
}

void BoundingCapsule::initialize(void) noexcept {
    BoundingVolume::initialize();

    if (!lineVAO) {
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
    }

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(Vertex), &lineVertices[0], GL_DYNAMIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void BoundingCapsule::deallocate(void) noexcept {
    BoundingVolume::initialize();

    if (lineVAO) {
        glDeleteVertexArrays(1, &lineVAO);
        glDeleteBuffers(1, &lineVBO);
        lineVAO = 0;
        lineVBO = 0;
    }
}

Line BoundingCapsule::getActualLine(void) const noexcept {
    const mat4 matrix = (Transform(transform.getTranslationAndRotation())).getMatrix();
    const GLfloat halfCapsuleLength = getActualCapsuleLineLength() * 0.5f;
    const vec3 topPoint = vec3(matrix * vec4(0.f, halfCapsuleLength, 0.f, 1.f));
    const vec3 bottomPoint = vec3(matrix * vec4(0.f, -halfCapsuleLength, 0.f, 1.f));

    return Line(bottomPoint, topPoint);
}

GLfloat BoundingCapsule::getActualRadius(void) const noexcept {
    const vec3 scaledRadii = glm::abs(transform.getScale()) * vec3(radius, radius, radius);
    return max(max(scaledRadii.x, scaledRadii.y), scaledRadii.z);
}

GLfloat BoundingCapsule::getActualCapsuleLineLength(void) const noexcept {
    return capsuleLineLength * glm::abs(transform.getScale().y);
}

vector<Vertex> BoundingCapsule::getVerticesForLine(void) const noexcept {
    vector<Vertex> lineVertices = vector<Vertex>(
        8,
        {
            vec3(0.f, 0.f, 0.f),
            vec3(1.f, 0.f, 0.f),
            vec3(1.f, 1.f, 1.f),
            vec2()
        }
    );

    lineVertices[0].position = vec3(1.f, -0.5f, 0.f);
    lineVertices[1].position = vec3(1.f, 0.5f, 0.f);
    
    lineVertices[2].position = vec3(-1.f, -0.5f, 0.f);
    lineVertices[3].position = vec3(-1.f, 0.5f, 0.f);

    lineVertices[4].position = vec3(0.f, -0.5f, 1.f);
    lineVertices[5].position = vec3(0.f, 0.5f, 1.f);
    
    lineVertices[6].position = vec3(0.f, -0.5f, -1.f);
    lineVertices[7].position = vec3(0.f, 0.5f, -1.f);

    return lineVertices;
}

vector<Vertex> BoundingCapsule::getVerticesForGrid(void) const noexcept {
    vector<Vertex> vertices = vector<Vertex>(
        BoundingCapsule::POINTS_PER_RING * 8,
        {
            vec3(0.f, 0.f, 0.f),
            vec3(1.f, 0.f, 0.f),
            vec3(1.f, 1.f, 1.f),
            vec2()
        }
    );

    constexpr GLfloat ANGLE_SPACING = 2.f * pi<GLfloat>() / static_cast<GLfloat>(BoundingCapsule::POINTS_PER_RING);
    constexpr GLint DOUBLE_POINTS_PER_RING = BoundingCapsule::POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_TWO = DOUBLE_POINTS_PER_RING * 2;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_THREE = DOUBLE_POINTS_PER_RING * 3;
    constexpr GLint DOUBLE_POINTS_PER_RING_TIMES_FOUR = DOUBLE_POINTS_PER_RING * 4;

    // ring centered along the x axis
    mat3x3 R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(1.f, 0.f, 0.f)));
    vertices[0].position = vec3(0.f, 0.f, 1.f);
    vertices[1].position = R * vertices[0].position;
    for (GLint i = 2; i < DOUBLE_POINTS_PER_RING; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i + 1].position = R * vertices[i].position;
    }

    // rings centered along the y axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 1.f, 0.f)));
    vertices[DOUBLE_POINTS_PER_RING].position = vec3(0.f, -0.000001f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING].normal = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING + 1].position = R * vertices[DOUBLE_POINTS_PER_RING].position;
    vertices[DOUBLE_POINTS_PER_RING + 1].normal = vec3(0.f, 1.f, 0.f);
    for (GLint i = DOUBLE_POINTS_PER_RING + 2; i < DOUBLE_POINTS_PER_RING_TIMES_TWO; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 1.f, 0.f)));
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position = vec3(0.f, 0.000001f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].normal = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].position = R * vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO].position;
    vertices[DOUBLE_POINTS_PER_RING_TIMES_TWO + 1].normal = vec3(0.f, 1.f, 0.f);
    for (GLint i = DOUBLE_POINTS_PER_RING_TIMES_TWO + 2; i < DOUBLE_POINTS_PER_RING_TIMES_THREE; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    // ring centered along the z axis
    R = mat3x3(glm::rotate(mat4(1.f), ANGLE_SPACING, vec3(0.f, 0.f, 1.f)));
    vertices[DOUBLE_POINTS_PER_RING_TIMES_THREE].position = vec3(0.f, 1.f, 0.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_THREE].normal = vec3(0.f, 0.f, 1.f);
    vertices[DOUBLE_POINTS_PER_RING_TIMES_THREE + 1].position = R * vertices[DOUBLE_POINTS_PER_RING_TIMES_THREE].position;
    vertices[DOUBLE_POINTS_PER_RING_TIMES_THREE + 1].normal = vec3(0.f, 0.f, 1.f);
    for (GLint i = DOUBLE_POINTS_PER_RING_TIMES_THREE + 2; i < DOUBLE_POINTS_PER_RING_TIMES_FOUR; i += 2) {
        vertices[i].position = vertices[i - 1].position;
        vertices[i].normal = vertices[i - 1].normal;
        vertices[i + 1].position = R * vertices[i].position;
        vertices[i + 1].normal = R * vertices[i].normal;
    }

    return vertices;
}

const GLfloat& BoundingCapsule::getCapsuleLineLength(void) const noexcept {
    return capsuleLineLength;
}

void BoundingCapsule::setCapsuleLineLength(const GLfloat& capsuleLineLength) noexcept {
    this->capsuleLineLength = capsuleLineLength;
}

const GLfloat& BoundingCapsule::getRadius(void) const noexcept {
    return radius;
}

void BoundingCapsule::setRadius(const GLfloat& radius) noexcept {
    this->radius = radius;
}

bool BoundingCapsule::intersectsVolume(BoundingVolume* boundingVolume) const noexcept {
    Line line = getActualLine();
    GLfloat radius = getActualRadius();

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = line.getClosestPtPointSegment(bCenter) - bCenter;
        const GLfloat radiusSum = radius + bSphere->getActualRadius();
        return dot(diff, diff) <= radiusSum * radiusSum;
    }
    // handle bounding capsule here
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        Line bLine = bCapsule->getActualLine();

        vec3 c1, c2;
        const GLfloat dist2 = line.getClosestPtSegmentSegment(c1, c2, bLine);
        const GLfloat radiusSum = radius + bCapsule->getActualRadius();
        return dist2 <= radiusSum * radiusSum;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->intersectsVolume(self);
}

bool BoundingCapsule::enclosesVolume(BoundingVolume* boundingVolume) const noexcept {
    Line line = getActualLine();
    GLfloat radius = getActualRadius();

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff = line.getClosestPtPointSegment(bCenter) - bCenter;
        const GLfloat fullDist = length(diff) + bSphere->getActualRadius();

        return fullDist <= radius;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        Line bLine = bCapsule->getActualLine();
        GLfloat bRadius = bCapsule->getActualRadius();
        
        const vec3 diff1 = line.getClosestPtPointSegment(bLine.getPointStart()) - bLine.getPointStart();
        const vec3 diff2 = line.getClosestPtPointSegment(bLine.getPointEnd()) - bLine.getPointEnd();
        const GLfloat fullDist1 = length(diff1) + bRadius;
        const GLfloat fullDist2 = length(diff2) + bRadius;

        return fullDist1 <= radius && fullDist2 <= radius;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->isEnclosedByVolume(self);
}

bool BoundingCapsule::isEnclosedByVolume(BoundingVolume* boundingVolume) const noexcept {
    Line line = getActualLine();
    GLfloat radius = getActualRadius();

    // handle bounding sphere here
    if (const BoundingSphere* bSphere = dynamic_cast<BoundingSphere*>(boundingVolume)) {
        const vec3 bCenter = bSphere->getCenter();
        const vec3 diff1 = line.getClosestPtPointSegment(line.getPointStart()) - bCenter;
        const vec3 diff2 = line.getClosestPtPointSegment(line.getPointEnd()) - bCenter;
        const GLfloat fullDist1 = length(diff1) + radius;
        const GLfloat fullDist2 = length(diff2) + radius;
        const GLfloat bRadius = bSphere->getActualRadius();

        return fullDist1 <= bRadius && fullDist2 <= bRadius;
    }
    // handle bounding capsule
    else if (const BoundingCapsule* bCapsule = dynamic_cast<BoundingCapsule*>(boundingVolume)) {
        Line bLine = bCapsule->getActualLine();

        const vec3 diff1 = bLine.getClosestPtPointSegment(line.getPointStart()) - line.getPointStart();
        const vec3 diff2 = bLine.getClosestPtPointSegment(line.getPointEnd()) - line.getPointEnd();
        const GLfloat fullDist1 = length(diff1) + radius;
        const GLfloat fullDist2 = length(diff2) + radius;
        const GLfloat bRadius = bCapsule->getActualRadius();

        return fullDist1 <= bRadius && fullDist2 <= bRadius;
    }

    // handle all others here
    BoundingVolume* self = (BoundingVolume*)this;
    return boundingVolume->enclosesVolume(self);
}

void BoundingCapsule::setCapsuleLineShader(const shared_ptr<Shader>& capsuleLineShader) noexcept {
    this->capsuleLineShader = capsuleLineShader;
}

void BoundingCapsule::draw(const mat4& ProjectionViewMatrix) const {
    if (shader != nullptr && capsuleLineShader != nullptr && this->renderVolume) {
        const mat4 model = (Transform(transform.getTranslationAndRotation()).getMatrix());
        const GLfloat radius = getActualRadius();
        const GLfloat capsuleLineLength = getActualCapsuleLineLength();

        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setFloat("radius", radius);
        shader->setFloat("capsuleLineLength", capsuleLineLength);

        glBindVertexArray(getVAO());
        glDrawArrays(GL_LINES, 0, (GLsizei)getVertices().size());
        glBindVertexArray(0);
    
        if (capsuleLineLength != 0.f) {
            capsuleLineShader->use();
            capsuleLineShader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
            capsuleLineShader->setMat4("model", value_ptr(model));
            capsuleLineShader->setFloat("radius", radius);
            capsuleLineShader->setFloat("capsuleLineLength", capsuleLineLength);

            glBindVertexArray(lineVAO);
            glDrawArrays(GL_LINES, 0, (GLsizei)lineVertices.size());
            glBindVertexArray(0);
        }
    }

    SceneObject::draw(ProjectionViewMatrix);
}