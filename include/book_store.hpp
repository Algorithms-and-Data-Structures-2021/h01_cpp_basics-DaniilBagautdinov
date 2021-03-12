#pragma once

#include <string>
#include <vector>

#include "author.hpp"
#include "book.hpp"

// перечисление: статус изменения размера хранилища книг
enum class ResizeStorageStatus {
  SUCCESS,                // увеличения размера хранилища прошло успешно
  NULL_STORAGE,           // передан нулевой указатель на хранилище
  INSUFFICIENT_CAPACITY,  // новый объем хранилища меньше или равен текущему кол-ву книг
  NEGATIVE_SIZE           // передано отрицательное кол-во книг в хранилище
};

/**
 * Изменение вместимости хранилища книг (динамического массива структур).
 * Книги из предыдущего хранилища переносятся (копируются) в хранилище нового размера.
 *
 * Заметьте, что новые элементы динамического массива не будут nullptr:
 * Before: arr = [] (size = 0, capacity = 0)
 * After: resize_storage(arr, 0, 3) => [{}, {}, {}] (size = 0, capacity = 3)
 * где {} - обозначает структуру с нулевыми полями (не nullptr)
 *
 * @param storage - ссылка на указатель на динамический массив книг (хранилище)
 * @param size - текущий размер хранилища (кол-во книг в хранилище)
 * @param new_capacity - новая вместимость хранилища
 * @return статус изменеия размера хранилища книг
 */
ResizeStorageStatus resize_storage(Book *&storage, int size, int new_capacity);

// структура: магазин книг
struct BookStore {
 public:
  /**
   * Создает объект книжного магазина.
   * Инициализирует название магазина переданной строкой.
   * Выделяет память под хранилище книг объемом kInitStorageCapacity.
   *
   * @param name - название книжного магазина
   */
  explicit BookStore(const std::string &name);

  /**
   * Высвобожадет выделенную память под хранилище книг.
   * Устанавливает значения кол-ва книг и объема хранилища в нулевые значения.
   * P.S. на ключевое слово virtual не обращайте внимания (необходимо для тестов).
   */
  virtual ~BookStore();

  /**
   * Добавление книги в хранилище магазина.
   * При нехватке места в хранилище его объем увеличивается на kCapacityCoefficient.
   *
   * @param book - книга, которую необходимо добавить в хранилище
   */
  void AddBook(const Book &book);

  // getters
  const std::string &GetName() const;
  int GetSize() const;
  int GetCapacity() const;
  const Book *GetBooks() const;

  // === необходимо для тестов ===
  BookStore() = default;
  friend bool operator==(const BookStore &lhs, const BookStore &rhs);
  friend bool operator!=(const BookStore &lhs, const BookStore &rhs);

 public:
  // константы (при желании, вы можете изменить их значения)
  static constexpr int kCapacityCoefficient = 5;   // коэффициент увеличения размера хранилища книг
  static constexpr int kInitStorageCapacity = 10;  // изначальный объем хранилища книг

 private:
  // поля структуры
  std::string name_;         // название магазина книг
  int storage_size_{0};      // кол-во книг в хранилище магазина
  int storage_capacity_{0};  // объем хранилища
  Book *storage_{nullptr};   // динамический массив - хранилище

  // приватный метод для увеличения объема хранилища
  ResizeStorageStatus resize_storage_internal(int new_capacity);
};

// === необходимо для тестов ===

inline bool operator==(const BookStore &lhs, const BookStore &rhs) {
  if (lhs.name_ != rhs.name_) return false;
  if (lhs.storage_size_ != rhs.storage_size_) return false;

  for (int index = 0; index < lhs.storage_size_; index++) {
    if (lhs.storage_[index] != rhs.storage_[index]) return false;
  }
  return true;
}

inline bool operator!=(const BookStore &lhs, const BookStore &rhs) {
  return !(lhs == rhs);
}

// внутренние проверки на этапе компиляции (не обращайте внимания)
static_assert(BookStore::kInitStorageCapacity >= 1);
static_assert(BookStore::kCapacityCoefficient >= 1);
static_assert(static_cast<int>(ResizeStorageStatus::NEGATIVE_SIZE) == 3);
