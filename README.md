Парсер метаинформации в формате ID3v2.4 из mp3 - файлов.

Реализовал консольное приложение для отображения метаинформации в формате ID3v2.4 из mp3-файлов.
Приложение считывает аргументы командной строки (именно туда передается название файла) и затема парсит метаданные в удобные структуры. С его помощью можно, например, узнать сколько длится данная песня, кто автор, в какой году выпущена и так далее.

Структуры:
1. MP3Parser - это структура для получаения, обработки и хранения тэгов. Она содержит в себе тэги
2. Тэг - структура, в которой хранится множество фрэймов. У тэга есть своя информация, такая как длина, версия (приложение обрабатывает только ID3v2.4), специальные флаги
3. Фрейм - единица информации. Их большое количество разных