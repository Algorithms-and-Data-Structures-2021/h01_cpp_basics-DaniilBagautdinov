#pragma once

#include <string>

// перечисление: биологический пол автора
enum class Sex {
  MALE,
  FEMALE,
  UNDEFINED  // Т - толерантность
};

// структура: автор книг
struct Author {
 public:
  /**
   * Создает объект автора книги.
   * Инициализирует поля объекта переданными значениями.
   *
   * @param full_name - полное имя автора книги
   * @param age - возраст автора
   * @param sex - биологический пол автора
   */
  Author(const std::string &full_name, int age, Sex sex);

  // setters
  void SetAge(int age);
  void SetSex(Sex sex);
  void SetFullName(const std::string &full_name);

  // getters
  int GetAge() const;
  Sex GetSex() const;
  const std::string &GetFullName() const;

  // === необходимо для тестов ===
  Author() = default;
  virtual ~Author() = default;
  friend bool operator==(const Author &lhs, const Author &rhs);
  friend bool operator!=(const Author &lhs, const Author &rhs);

 public:
  // константа для валидации возраста - минимальный возраст автора
  static constexpr int kMinAuthorAge = 16;

 private:
  // поля структуры
  std::string full_name_;    // полное имя

  int age_{0};               // возраст (измеряется в годах)
  Sex sex_{Sex::UNDEFINED};  // биологический пол
};

// === необходимо для тестов ===

inline bool operator==(const Author &lhs, const Author &rhs) {
  if (lhs.age_ != rhs.age_) return false;
  if (lhs.sex_ != rhs.sex_) return false;
  if (lhs.full_name_ != rhs.full_name_) return false;
  return true;
}

inline bool operator!=(const Author &lhs, const Author &rhs) {
  return !(lhs == rhs);
}

// внутренние проверки на этапе компиляции (не обращайте внимания)
static_assert(Author::kMinAuthorAge >= 0, "Author's min age must be non-negative");
static_assert(static_cast<int>(Sex::UNDEFINED) == 2, "Sex enumeration is out of range. Please, do not modify it!");
