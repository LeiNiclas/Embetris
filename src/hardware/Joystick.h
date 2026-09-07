#pragma once

class Joystick {
    public:
        void init();
        void readValues(float &x, float &y, bool &pressed);
        void readAverageValues(float &x, float &y);
        bool isLeft();
        bool isRight();
        bool isUp();
        bool isDown();
    
    private:
        float recentValuesX[10];
        float recentValuesY[10];
        int runningIndex;
};