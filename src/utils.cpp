#include "utils.h"

// LEVEL POS
LevelPos LevelPos::operator+(const LevelPos& other) const {
    LevelPos res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

LevelPos LevelPos::operator+(const LevelVelocity& other) const {
    LevelPos res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

LevelPos LevelPos::operator+=(const LevelVelocity& other) const {
    LevelPos res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

LevelPos LevelPos::operator-(const LevelPos& other) const {
    LevelPos res = *this;
    res.x -= other.x;
    res.y -= other.y;
    return res;
}

LevelPos LevelPos::operator/(const int& other) const {
    LevelPos res = *this;
    res.x /= other;
    res.y /= other;
    return res;
}

bool LevelPos::operator==(const LevelPos& other) const {
    return (x == other.x && y == other.y);
}

LevelRect LevelPos::makeRect(LevelSize size) const {
    return { x, y, size.w, size.h };
}



// LEVEL POS HASH
std::size_t LevelPosHash::operator()(const LevelPos& pos) const {
    return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
}



// LEVEL SIZE
LevelSize LevelSize::operator+(const LevelSize& other) const {
    LevelSize res = *this;
    res.w += other.w;
    res.h += other.h;
    return res;
}

LevelSize LevelSize::operator/(const int& other) const {
    LevelSize res = *this;
    res.w /= other;
    res.h /= other;
    return res;
}

LevelRect LevelSize::makeRect(LevelPos pos) const {
    return { pos.x, pos.y, w, h };
}



// LEVEL VELOCITY
bool LevelVelocity::isNull() const {
    return (x == 0.0f && y == 0.0f);
}

LevelVelocity LevelVelocity::operator*(const float& other) const {
    LevelVelocity res = *this;
    res.x *= other;
    res.y *= other;
    return res;
}

bool LevelVelocity::operator>(const LevelVelocity& other) const {
    return (std::abs(x) + std::abs(y) > std::abs(other.x) + std::abs(other.y));
}

// LEVEL RECT
LevelRect LevelRect::operator+(const LevelPos& other) const {
    LevelRect res = *this;
    res.x += other.x;
    res.y += other.y;
    return res;
}

LevelRect LevelRect::operator+(const float& other) const {
    LevelRect res = *this;
    res.x -= other;
    res.y -= other;
    res.w += other * 2.0f;
    res.h += other * 2.0f;
    return res;
}

Face LevelRect::collides(const LevelRect& other) const {
    bool collides = (
        other.x + other.w >= x &&
        other.x <= x + w &&
        other.y + other.h >= y &&
        other.y <= y + h
    );

    if (collides) {
        float overlapLeft = other.x + other.w - x;
        float overlapRight = x + w - other.x;
        float overlapTop = other.y + other.h - y;
        float overlapBottom = y + h - other.y;

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapLeft) return LEFT;
        else if (minOverlap == overlapRight) return RIGHT;
        else if (minOverlap == overlapTop) return TOP;
        else return BOTTOM;
    } else return NONE;
}

LevelPos LevelRect::getPos() const {
    return { x, y };
}

LevelSize LevelRect::getSize() const {
    return { w, h };
}

LevelPos LevelRect::getCenter() const {
    return { x + w / 2, y + h / 2 };
}



// OSTREAM
std::ostream& operator<<(std::ostream& os, const LevelPos& pos) {
    os << "x: " << pos.x << " y: " << pos.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const LevelSize& size) {
    os << "w: " << size.w << " h: " << size.h;
    return os;
}

std::ostream& operator<<(std::ostream& os, const LevelVelocity& velocity) {
    os << "x: " << velocity.x << " y: " << velocity.y;
    return os;
}

std::ostream& operator<<(std::ostream& os, const LevelRect& rect) {
    os << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h;
    return os;
}
