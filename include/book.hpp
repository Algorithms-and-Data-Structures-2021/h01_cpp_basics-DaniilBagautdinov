#pragma once

#include <string>
#include <vector>

#include "author.hpp"  // Author

// перечисление: жанр книги
enum class Genre {
  ACTION_AND_ADVENTURE,
  CLASSIC,
  DRAMA,
  FANTASY,
  SCI_FI,
  HORROR,
  ROMANCE,
  ADULT,
  THRILLER,
  BIOGRAPHY,
  HISTORY,
  POETRY,
  UNDEFINED
};

// перечисление: издательство книги
enum class Publisher {
  USA,
  RUS,
  ENG,
  AUS,
  UNDEFINED
};

// структура: книга
struct Book {
 public:
  /**
   * Создает объект книги.
   * Инициализирует поля объекта переаданными значениями.
   *
   * @param title - название книги
   * @param content - содержание
   * @param genre - жанр
   * @param publisher - издательство
   * @param authors - список авторов
   */
  Book(const std::string &title,
       const std::string &content,
       Genre genre,
       Publisher publisher,
       const std::vector<Author> &authors);

  /**
   * Добавление автора к списку авторов.
   * Автор с уже существующим в списке авторов имененем игнорируется.
   *
   * @param author - добавляемый автор книги
   * @return true - при успешном добавлении автора, false - в списке авторов обнаружен дубликат
   */
  bool AddAuthor(const Author &author);

  // getters
  const std::string &GetTitle() const;
  const std::string &GetContent() const;
  Genre GetGenre() const;
  Publisher GetPublisher() const;
  const std::vector<Author> &GetAuthors() const;

  // setters
  void SetTitle(const std::string &title);
  void SetContent(const std::string &content);
  void SetGenre(Genre genre);
  void SetPublisher(Publisher publisher);

  // === необходимо для тестов ===
  Book() = default;
  virtual ~Book() = default;
  friend bool operator==(const Book &lhs, const Book &rhs);
  friend bool operator!=(const Book &lhs, const Book &rhs);

 private:
  // поля структуры
  std::string title_;                          // название
  std::string content_;                        // содержание

  std::vector<Author> authors_;                // список авторов

  Genre genre_{Genre::UNDEFINED};              // жанр
  Publisher publisher_{Publisher::UNDEFINED};  // издательсво
};

// === необходимо для тестов ===

inline bool operator==(const Book &lhs, const Book &rhs) {
  if (lhs.title_ != rhs.title_) return false;
  if (lhs.content_ != rhs.content_) return false;
  if (lhs.authors_ != rhs.authors_) return false;
  if (lhs.genre_ != rhs.genre_) return false;
  if (lhs.publisher_ != rhs.publisher_) return false;
  return true;
}

inline bool operator!=(const Book &lhs, const Book &rhs) {
  return !(lhs == rhs);
}

// внутренние проверки на этапе компиляции (не обращайте внимания)
static_assert(static_cast<int>(Genre::UNDEFINED) == 12);
static_assert(static_cast<int>(Publisher::UNDEFINED) == 4);
