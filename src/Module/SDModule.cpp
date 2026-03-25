#include "SDModule.h"

bool SDModule::begin()
{
    if (!sd.begin(SdSpiConfig(SD_CS, SHARED_SPI, SD_SCK_MHZ(16))))
    {
        Serial.println("SD fail");
        return false;
    }
    else
    {
        Serial.println("SD Connect. SD OK!");
        return true;
    }
}
void SDModule::ListFiles(const char *dirname) {
    FsFile dir;
    FsFile file;
    char fileName[256]; // Буфер для длинного имени

    if (!dir.open(dirname)) {
        Serial.println("Ошибка открытия директории!");
        return;
    }

    while (file.openNext(&dir, O_RDONLY)) {
        file.getName(fileName, sizeof(fileName)); // Вот она — ПОБЕДА над 8.3!

        if (file.isDir()) {
            Serial.print("  DIR : ");
            Serial.println(fileName);
        } else {
            Serial.print("  FILE: ");
            Serial.print(fileName);
            Serial.print("  SIZE: ");
            Serial.println((uint32_t)file.fileSize());
        }
        file.close(); // В SdFat обязательно закрывать файл в цикле!
    }
    dir.close();
}

void SDModule::CreateFile(const char* file_name) {
    FsFile file;
    // O_RDWR | O_CREAT | O_AT_END - открыть для записи, создать если нет, писать в конец
    if (file.open(file_name, O_RDWR | O_CREAT | O_AT_END)) {
        file.println("Log start...");
        file.close();
        Serial.println("File Created/Opened OK");
    } else {
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