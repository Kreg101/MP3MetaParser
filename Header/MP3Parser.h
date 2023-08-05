#include "Tag.h"
#include <map>

class MP3Parser {
private:
    // набор тэгов
    std::vector<Tag*> tags;

    // обработка конкретного фрэйма внутри тэга
    void FrameHandling(Frame* frame, unsigned int& tag_size, std::ifstream& file_in, bool unknown_frame);

public:

    // Parse обрабатывает аргументы командной строки
    void Parse(int argc, char** argv);

    // Выводит содержимое тэгов
    [[nodiscard]] std::fstream& Output() const;

    // Выводит содержимое тэгов в виде таблицы
    std::fstream& TableOutput();

    // Получаем конкретный тэг по ключу
    [[nodiscard]] Tag* GetTag(const std::string& tag) const;

};
