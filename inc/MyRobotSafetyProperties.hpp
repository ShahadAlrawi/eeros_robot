#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doNext;
    eeros::safety::SafetyEvent doAbort;
    eeros::safety::SafetyEvent doShutdown;
    eeros::safety::SafetyEvent doStart;
    eeros::safety::SafetyEvent doRestart;


    // Defina all possible levels
    eeros::safety::SafetyLevel slEmergency;
    eeros::safety::SafetyLevel slEmergencyShuttingDown;
    eeros::safety::SafetyLevel slEmergencyMotorStop;
    eeros::safety::SafetyLevel slSystemOFF;
    eeros::safety::SafetyLevel slShuttingDown;
    eeros::safety::SafetyLevel slMotorStop;
    eeros::safety::SafetyLevel slSystemIdle;
    eeros::safety::SafetyLevel slSystemInit;
    eeros::safety::SafetyLevel slMotorStart;
    eeros::safety::SafetyLevel slSystemON;
private:
    // Define all critical outputs
    eeros::hal::Output<bool>* ledGreen;
    eeros::hal::Output<bool>* ledRed;
    eeros::hal::Output<bool>* led0;

    // Define all critical inputs
    eeros::hal::Input<bool>* bMode;
    eeros::hal::Input<bool>* bPause;


    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
