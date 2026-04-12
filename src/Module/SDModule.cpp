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

void SDModule::ListFiles()
{
    File32 root;
    File32 file;

    if (!root.open("/"))
    {
        Serial.println("Root open fail");
        return;
    }

    while (file.openNext(&root, O_RDONLY))
    {
        char name[64];
        file.getName(name, sizeof(name));

        Serial.print(name);
        Serial.print("  ");
        Serial.println(file.fileSize());

        file.close();
    }

    root.close();
}

int SDModule::ReadValFromFile(const char *file_name)
{
    File32 file;
    if (!file.open(file_name, O_RDONLY))
    {
        Serial.print("Ошибка: файл ");
        Serial.print(file_name);
        Serial.println(" не найден!");
        return -1;
    }

    char buf[8];
    memset(buf, 0, sizeof(buf));
    int err = file.read(buf, sizeof(buf) - 1);
    file.close();

    if (err > 0)
    {
        int count = atoi(buf);
        return count;
    }
    return -1;
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
