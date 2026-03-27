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