# Парсер метаинформации в формате ID3v2.4 из mp3 - файлов.

## Описание

Реализовал консольное приложение для отображения метаинформации в формате ID3v2.4 из mp3-файлов.
Помимо удобного интерфейса для работы метаинформацией в приложении реализован форматированный вывод и сохранение в файл в формате csv.

## ID3v2.4

Детальную спецификацию о структуре ID3v2.4 можно найти в [документации](docs/id3v2.4.0-structure.rst)
Различная по смыслу информации (название, артист, жанр и тд) хранятся в различных фреймах. Список возможных фреймов можно также найти в [документации](docs/id3v2.4.0-frames.rst)

[Оригинальная документация](https://mutagen-specs.readthedocs.io/en/latest/id3/index.html)

## Реализация

Все фреймы поделил на разные категории (как сделано в документации). Структура **MP3Parser** взаимодействует с файлами, сохраняя данные в струтуру **Tag**. В свою очередь в **Tag** находятся **Frame**. **Frame** - абстрактный класс, содержащий общие для всех фрэймов, наследующихся от него, методы и поля. Фреймы разделены на категории в связи со своими особенностями. Все реализовано интуитивно понятно, аналогично с оригинальной документацией.
Реализована следующая иерархия классов для описания всех фреймов.
![uml]{https://github.com/Kreg101/MP3MetaParser/blob/main/Uml/ClassesDiagram.png}







