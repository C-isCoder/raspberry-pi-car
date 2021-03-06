//
//  car.c
//  raspberry-pi-car
//
//  Created by 丛晓丹 on 2019/2/19.
//  Copyright © 2019 丛晓丹. All rights reserved.
//

#include "car.h"

//int main(void) {
//    init_car();
//    int input;
//    for (;;) {
//        input = scan_input();
//        printf("input commond :%c\n", input);
//        do_commond(input);
//    }
//    return 0;
//}

void init_car(){
    printf("initCar\n");
    wiringPiSetup();
    // 超声波
    pinMode(ULTRASONIC_TRIGGER, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);
    pinMode(ULTRASONIC_GEAR, OUTPUT);
    // L298N PWM
    softPwmCreate (EN_A ,0, 100);
    softPwmCreate (EN_B ,0, 100);
    // Motor
    pinMode(IN_1, OUTPUT);
    pinMode(IN_2, OUTPUT);
    pinMode(IN_3, OUTPUT);
    pinMode(IN_4, OUTPUT);
    // defaule
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
}

void forward(){
    printf("forward\n");
    // pwm
    softPwmWrite(EN_A, 100);
    softPwmWrite(EN_B, 100);
    // left
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    // right
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void right(){
    printf("right\n");
    // pwm
    softPwmWrite(EN_A, 60);
    softPwmWrite(EN_B, 30);
    // left
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    // right
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
}

void left(){
    printf("left\n");
    // pwm
    softPwmWrite(EN_A, 30);
    softPwmWrite(EN_B, 60);
    // left
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    // right
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
}

void back(){
    printf("back\n");
    // pwm
    softPwmWrite(EN_A, 50);
    softPwmWrite(EN_B, 50);
    // left
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    // right
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
}

void stop(){
    printf("stop\n");
    // pwm
    softPwmWrite(EN_A, 0);
    softPwmWrite(EN_B, 0);
    // left
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    // right
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
}

int scan_input(){
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    
    in = getchar();
    
    tcsetattr(0, TCSANOW, &stored_settings);
    return in;
}

void do_commond(int commond){
    switch (commond) {
        case FORWARD: // forward
            stop();
            forward();
            break;
        case LEFT: // left
            stop();
            left();
            break;
        case RIGHT: // right
            stop();
            right();
            break;
        case BACK: // back
            stop();
            back();
            break;
        case STOP: // stop
            stop();
            break;
        default:
            stop();
            break;
    }
}

double ultrasonic_distance(){
    double distance;
    digitalWrite(ULTRASONIC_TRIGGER, HIGH);
    delayMicroseconds(10); // 给触发脚高电平10μs，这里至少是10μs
    digitalWrite(ULTRASONIC_TRIGGER, LOW);    // 持续给触发脚低电
    unsigned int echo_start = millis();
    while(digitalRead(ULTRASONIC_ECHO) == LOW && millis() - echo_start < 1000);
    // 等待低电平结束
    if (millis() - echo_start < 1000) {
        unsigned int start = micros();
        // 等待高电平结束
        while(digitalRead(ULTRASONIC_ECHO) == HIGH);
        unsigned int end = micros();
        unsigned int delta = end - start;
        // 声波速度取 340.29m/s   34029cm/s
        // 测得距离(单位:m)  =  delta * 声波速度 / 2
        // 测得距离(单位:cm)  =  delta * 声波速度 / 2 * 100
        distance = 34029 * delta / 2000000.0;
        printf("distance: %f cm\n", distance);
    }
    return distance;
}
