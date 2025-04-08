#pragma once
class Grade {
protected:
    float internalMark;
    float finalMark;
public:
    virtual ~Grade() = default;
    virtual float calculateGrade() const = 0;
    void setInternalMark(float iMark);
    void setFinalMark(float fMark);

    // ADDED: Accessor methods for internal and final marks
    float getInternalMark() const { return internalMark; }
    float getFinalMark() const { return finalMark; }
};

class PrimaryGrade : public Grade {
public:
    float calculateGrade() const override;
};