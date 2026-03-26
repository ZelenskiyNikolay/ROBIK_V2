#include "SDModule.h"

bool SDModule::begin()
{
    if (!sd.begin(SD_CS, SD_SCK_MHZ(4)))
    {
        Serial.println("SD fail");
        return false;
    }
    else
    {
        Serial.println("SD Connect. SD OK!");
        Serial.print("Тип ФС: ");
        Serial.println((int)sd.fatType()); // Покажет 32 для FAT32, 16 для FAT16
        return true;
    }
}



void SDModule::ListFiles() {
    File32 root;
    File32 file;

    if (!root.open("/")) {
        Serial.println("Root open fail");
        return;
    }

    while (file.openNext(&root, O_RDONLY)) {
        char name[64];
        file.getName(name, sizeof(name));

        Serial.print(name);
        Serial.print("  ");
        Serial.println(file.fileSize());

        file.close();
    }

    root.close();
}

// void SDModule::ListFiles(const char *dirname)
// {
//     FsFile dir;
//     FsFile entry;
//     char fileName[256];

//     Serial.print("Тип ФС: ");
//     Serial.println((int)sd.fatType()); // Покажет 32 для FAT32, 16 для FAT16

//     // В Adafruit Fork/SdFat V2 директория открывается ТАК:
//     if (!dir.open(dirname, O_RDONLY))
//     {
//         Serial.print("Ошибка открытия пути: ");
//         Serial.println(dirname);
//         Serial.print("Код ошибки SdFat: 0x");
//         Serial.println(sd.sdErrorCode(), HEX);
//         return;
//     }

//     if (!dir.isDir())
//     {
//         Serial.println("Объект не является директорией!");
//         dir.close();
//         return;
//     }

//     dir.rewind(); // Возвращаемся в начало списка файлов

//     Serial.println("--- Содержимое: ---");

//     // Перебор файлов внутри директории 'dir'
//     while (entry.openNext(&dir, O_RDONLY))
//     {
//         entry.getName(fileName, sizeof(fileName));

//         if (entry.isDir())
//         {
//             Serial.print("  [DIR]  ");
//             Serial.println(fileName);
//         }
//         else
//         {
//             Serial.print("  [FILE] ");
//             Serial.print(fileName);
//             Serial.print("  SIZE: ");
//             Serial.println((uint32_t)entry.fileSize());
//         }
//         entry.close(); // Обязательно закрываем entry на каждой итерации!
//     }
//     dir.close();
//     Serial.println("-------------------");
// }


// void SDModule::ListFiles(const char *dirname) {
//     FsFile dir;
//     FsFile file;
//     char fileName[256]; // Буфер для длинного имени

//     if (!dir.open(dirname)) {
//         Serial.println("Ошибка открытия директории!");
//         return;
//     }

//     while (file.openNext(&dir, O_RDONLY)) {
//         file.getName(fileName, sizeof(fileName)); // Вот она — ПОБЕДА над 8.3!

//         if (file.isDir()) {
//             Serial.print("  DIR : ");
//             Serial.println(fileName);
//         } else {
//             Serial.print("  FILE: ");
//             Serial.print(fileName);
//             Serial.print("  SIZE: ");
//             Serial.println((uint32_t)file.fileSize());
//         }
//         file.close(); // В SdFat обязательно закрывать файл в цикле!
//     }
//     dir.close();
// // }
// void SDModule::ListFiles(const char *dirname) {
//     FsFile dir;
//     FsFile file;
//     char fileName[256];

//     // Если dirname пришел пустой, принудительно ставим корень
//     const char* path = (dirname && strlen(dirname) > 0) ? dirname : "/";

//     Serial.print("--- Открываем путь: ");
//     Serial.println(path);

//     if (!dir.open(path)) {
//         Serial.print("Ошибка открытия директории! Код ошибки SdFat: 0x");
//         Serial.println(sd.sdErrorCode(), HEX); // СМОТРИМ СЮДА В МОНИТОР ПОРТА
//         return;
//     }

//     // Добавим перемотку в начало, на всякий случай
//     dir.rewind();

//     int count = 0;
//     while (file.openNext(&dir, O_RDONLY)) {
//         count++;
//         file.getName(fileName, sizeof(fileName));

//         if (file.isDir()) {
//             Serial.print("  [DIR]  ");
//             Serial.println(fileName);
//         } else {
//             Serial.print("  [FILE] ");
//             Serial.print(fileName);
//             Serial.print("  SIZE: ");
//             Serial.println((uint32_t)file.fileSize());
//         }
//         file.close();
//     }

//     if (count == 0) Serial.println("...Директория пуста...");
//     dir.close();
// }

void SDModule::CreateFile(const char *file_name)
{
    FsFile file;
    // O_RDWR | O_CREAT | O_AT_END - открыть для записи, создать если нет, писать в конец
    if (file.open(file_name, O_RDWR | O_CREAT | O_AT_END))
    {
        file.println("Log start...");
        file.close();
        Serial.println("File Created/Opened OK");
    }
    else
    {
        Serial.println("CreateFile Fail");
    }
}

// float SDModule::getFloatConfig(const char *key, float defaultValue)
// {
//     File configFile = SD.open("config.txt");
//     if (!configFile)
//     {
//         if (CreateConfigFile())
//             return defaultValue;
//     }

//     float result = defaultValue;
//     while (configFile.available())
//     {
//         String line = configFile.readStringUntil('\n');
//         line.trim();

//         if (line.startsWith(key))
//         {
//             int colonIndex = line.indexOf(':');
//             if (colonIndex != -1)
//             {
//                 String valuePart = line.substring(colonIndex + 1);
//                 valuePart.trim();
//                 result = valuePart.toFloat();
//                 break; // Нашли, выходим
//             }
//         }
//     }
//     configFile.close();
//     return result;
// }

// int SDModule::getIntConfig(const char *key, int defaultValue)
// {
//     return (int)getFloatConfig(key, (float)defaultValue);
// }
// bool SDModule::CreateConfigFile()
// {
//     Serial.println(F("Writing /config.txt ..."));
//     File file = SD.open("config.txt", FILE_WRITE);
//     if (file)
//     {
//         file.println("speed: 180");
//         file.println("angle: 90");
//         file.close();
//         Serial.println(F("Create config.txt"));
//         return true;
//     }
//     else
//     {
//         Serial.println(F("Failed to open config.txt for writing!!! "));
//         return false;
//     }
// }
// void SDModule::CreateFile(const char *file_name)
// {
//     File file = SD.open(file_name);
//     if (!file) // Проверяем вдруг существует
//     {
//         file = SD.open(file_name, FILE_WRITE);
//         if (file)
//         {
//             file.close();
//             Serial.print("Файл создан: ");
//             Serial.println(file_name);
//             return;
//         }
//     }
//     else
//     {
//         file.close();
//         Serial.print("Файл существует: ");
//         Serial.println(file_name);
//         return;
//     }
// }