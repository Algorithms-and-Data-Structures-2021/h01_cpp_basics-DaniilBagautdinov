#include "author.hpp"

#include <stdexcept>  // invalid_argument

// 1. реализуйте конструктор ...
Author::Author(const std::string &full_name, int age, Sex sex) {
  // валидация аргументов (здесь был Рамиль)
  if (age < kMinAuthorAge) {
    throw std::invalid_argument("Author::age must be greater than " + std::to_string(kMinAuthorAge));
  }

  if (full_name.empty()) {
    throw std::invalid_argument("Author::full_name must not be empty");
  }
  // Tip 1: инициализируйте поля
}

void Author::SetAge(int age) {
  if (age < kMinAuthorAge) {
    throw std::invalid_argument("Author::age must be greater than " + std::to_string(kMinAuthorAge));
  }
  age_ = age;
}

void Author::SetSex(Sex sex) {
  sex_ = sex;
}

void Author::SetFullName(const std::string &full_name) {
  if (full_name.empty()) {
    throw std::invalid_argument("Author::full_name must not be empty");
  }
  full_name_ = full_name;
}

int Author::GetAge() const {
  return age_;
}

Sex Author::GetSex() const {
  return sex_;
}

const std::string &Author::GetFullName() const {
  return full_name_;
}
