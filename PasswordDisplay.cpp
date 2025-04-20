#include "PasswordDisplay.h"

std::string PasswordDisplay::maskPassword(const std::string& password) {
    return std::string(password.length(), '*');
}