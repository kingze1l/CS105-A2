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
};

class PrimaryGrade : public Grade {
public:
    float calculateGrade() const override;
};
