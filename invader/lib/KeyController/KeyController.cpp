#include "PCF8574.h"
#include "KeyController.h"

#ifdef ESP8266
#include "FS.h"
#else
#include <EEPROM.h>
#endif

bool KeyController::begin(int mode)
{   

    if (mode == USE_PCF)
    {
        keyExt.begcdin();
        keyExt.pinMode(up, INPUT_PULLUP);    // giù
        keyExt.pinMode(down, INPUT_PULLUP);  // su
        keyExt.pinMode(a, INPUT_PULLUP);     //A
        keyExt.pinMode(b, INPUT_PULLUP);     // B
        keyExt.pinMode(right, INPUT_PULLUP); // dx
        keyExt.pinMode(left, INPUT_PULLUP);  // sx
    }
    else
    {
        pinMode(up, INPUT_PULLUP);    // giù
        pinMode(down, INPUT_PULLUP);  // su
        pinMode(a, INPUT_PULLUP);     //A
        pinMode(b, INPUT_PULLUP);     // B
        pinMode(right, INPUT_PULLUP); // dx
        pinMode(left, INPUT_PULLUP);  // sx
    }
    KeyController::mode = mode;
}

#ifdef ESP8266
void KeyController::saveKeyMap()
{
    Serial.begin(9600);
    if (SPIFFS.begin())
    {
        char button[][6] = {"a", "b", "up", "down", "right", "left"};
        File file = SPIFFS.open("/button.conf", "w");

        for (size_t i = 0; i < 6; i++)
        {
            Serial.print("insert pin button ");
            Serial.print(button[i]);
            Serial.println(" : ");

            while (Serial.available() == 0)
                ;

            String readed = Serial.readString();
            file.println(readed.toInt());
        }
    }
    else
    {
        Serial.print("memory acces faild");
    }
}

void KeyController::loadKeyMap()
{
    Serial.begin(9600);
    if (SPIFFS.begin())
    {
        char button[][6] = {"a", "b", "up", "down", "right", "left"};
        int buttonVal[6];

        File file = SPIFFS.open("/button.conf", "r");
        if (!file)
        {
            Serial.print("file not exist");
            return;
        }

        for (size_t i = 0; i < 6; i++)
        {
            Serial.print("insert pin button ");
            Serial.print(button[i]);
            Serial.print(" : ");
            int val = file.parseInt();
            Serial.println(val);
            buttonVal[i] = val;
        }

        up = buttonVal[2];
        down = buttonVal[3];
        left = buttonVal[5];
        right = buttonVal[4];
        a = buttonVal[0];
        b = buttonVal[1];
    }
    else
    {
        Serial.print("memory acces faild");
    }
}
#else
void KeyController::saveKeyMap()
{
    Serial.begin(9600);
    char button[][6] = {"a", "b", "up", "down", "right", "left"};

    for (size_t i = 0; i < 6; i++)
    {
        Serial.print("insert pin button ");
        Serial.print(button[i]);
        Serial.println(" : ");

        while (Serial.available() == 0)
            ;

        String readed = Serial.readString();
        uint8_t val = readed.toInt();
        EEPROM.write(i, val);
    }
}

void KeyController::loadKeyMap()
{
    Serial.begin(9600);

    char button[][6] = {"a", "b", "up", "down", "right", "left"};
    int buttonVal[6];

    for (size_t i = 0; i < 6; i++)
    {
        Serial.print("insert pin button ");
        Serial.print(button[i]);
        Serial.print(" : ");
        int val = EEPROM.read(i);
        Serial.println(val);
        buttonVal[i] = val;
    }

    up = buttonVal[2];
    down = buttonVal[3];
    left = buttonVal[5];
    right = buttonVal[4];
    a = buttonVal[0];
    b = buttonVal[1];
}

#endif

bool KeyController::isUpPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(up) == LOW)
        {
            return true;
        }
        return false;
    }

    if (keyExt.digitalRead(up) == LOW)
    {
        return true;
    }
    return false;
}
bool KeyController::isDownPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(down) == LOW)
        {
            return true;
        }
        return false;
    }
    if (keyExt.digitalRead(down) == LOW)
    {
        return true;
    }
    return false;
}
bool KeyController::isLeftPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(left) == LOW)
        {
            return true;
        }
        return false;
    }

    if (keyExt.digitalRead(left) == LOW)
    {
        return true;
    }
    return false;
}
bool KeyController::isRightPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(right) == LOW)
        {
            return true;
        }
        return false;
    }

    if (keyExt.digitalRead(right) == LOW)
    {
        return true;
    }
    return false;
}
bool KeyController::isAPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(a) == LOW)
        {
            return true;
        }
        return false;
    }
    if (keyExt.digitalRead(a) == LOW)
    {
        return true;
    }
    return false;
}
bool KeyController::isBPressed()
{
    if (mode == USE_MIC_PIN)
    {
        if (digitalRead(b) == LOW)
        {
            return true;
        }
        return false;
    }

    if (keyExt.digitalRead(b) == LOW)
    {
        return true;
    }
    return false;
}

void KeyController::onUpPress(void (*up)())
{
    upCallBack = up;
}
void KeyController::onDownPress(void (*down)())
{
    downCallBack = down;
}
void KeyController::onLeftPress(void (*left)())
{
    leftCallBack = left;
}
void KeyController::onRightPress(void (*right)())
{
    rightCallBack = right;
}
void KeyController::onAPress(void (*a)())
{
    aCallBack = a;
}
void KeyController::onBPress(void (*b)())
{
    bCallBack = b;
}
bool KeyController::buttonEventListener()
{
    if (upCallBack != NULL && isUpPressed())
        upCallBack();
    if (downCallBack != NULL && isDownPressed())
        downCallBack();
    if (leftCallBack != NULL && isLeftPressed())
        leftCallBack();
    if (rightCallBack != NULL && isRightPressed())
        rightCallBack();
    if (aCallBack != NULL && isAPressed())
        aCallBack();
    if (bCallBack != NULL && isBPressed())
        bCallBack();
}