#include "Grade.h"

void Grade::setInternalMark(float iMark) { internalMark = iMark; }
void Grade::setFinalMark(float fMark) { finalMark = fMark; }

float PrimaryGrade::calculateGrade() const {
    return 0.3f * internalMark + 0.7f * finalMark;
}