#include "MyRobotSafetyProperties.hpp"

/*
The system safety levels consists of:

        (Emergency Shutdown)
0 - slEmergency
1 - slEmergencyShuttingDown
2 - slEmergencyMotorStop
----------------------------
        (Gracefull Shutdown)
3 - slSystemOFF
4 - slShuttingDown
5 - slMotorStop
----------------------------
        (Startup)
6 - slSystemIdle
7 - slSystemInit
8 - slMotorStart
9 - slSystemON
---------------------------

LedGreen: Show if system is in Startup
    ON in level 6 - 8
    OFF otherwise

LedRed: Show if system is in emergency
    ON in level 0 - 2
    OFF otherwise

bMode: Restart (restart the system from emergency or off state)
bPause: Abort (can be triggered in levels 3 - 8 to start move to emergency)
*/
MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs), 
    // Member initializers - Safety Events
    doNext("doNext"),
    doAbort("doAbort"),
    doShutdown("doShutdown"),
    doStart("doStart"),
    doRestart("doRestart"),

    // Member initializers - Safety Levels
    slEmergency("slEmergency"),
    slEmergencyShuttingDown("slEmergencyShuttingDown"),
    slEmergencyMotorStop("slEmergencyMotorStop"),
    slSystemOFF("slSystemOF"),
    slShuttingDown("slShuttingDown"),
    slMotorStop("slMotorStop"),
    slSystemIdle("slSystemIdle"),
    slSystemInit("slSystemInit"),
    slMotorStart("slMotorStart"),
    slSystemON("slSystemON")
{
    // creating a reference to the hal instance
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    ledRed = hal.getLogicOutput("onBoardLEDred");
    ledGreen = hal.getLogicOutput("onBoardLEDgreen");
    led0 = hal.getLogicOutput("onBoardLEDuser0");
    criticalOutputs = {ledRed, ledGreen, led0};

    // Declare and add critical inputs
    bMode = hal.getLogicInput("onBoardButtonMode", false);
    bPause = hal.getLogicInput("onBoardButtonPause", false);
    criticalInputs = {bMode, bPause};

    // Add all safety levels to the safety system
    addLevel(slEmergency);
    addLevel(slEmergencyShuttingDown);
    addLevel(slEmergencyMotorStop);
    addLevel(slSystemOFF);
    addLevel(slShuttingDown);
    addLevel(slMotorStop);
    addLevel(slSystemIdle);
    addLevel(slSystemInit);
    addLevel(slMotorStart);
    addLevel(slSystemON);


    // Add events to individual safety levels
    // Currently all events are private except doStart and doAbort
    slEmergency.addEvent(doRestart, slSystemInit, kPublicEvent);
    slEmergencyShuttingDown.addEvent(doNext, slEmergency, kPrivateEvent);
    slEmergencyMotorStop.addEvent(doNext, slEmergencyShuttingDown, kPrivateEvent);

    slShuttingDown.addEvent(doNext, slSystemOFF, kPrivateEvent);
    slMotorStop.addEvent(doNext, slShuttingDown, kPrivateEvent);
    
    slSystemIdle.addEvent(doStart, slSystemInit, kPublicEvent);
    slSystemInit.addEvent(doNext, slMotorStart, kPrivateEvent);
    slMotorStart.addEvent(doNext, slSystemON, kPrivateEvent);

    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slSystemOFF, slSystemON, doAbort, slEmergencyMotorStop, kPublicEvent);
    addEventToAllLevelsBetween(slSystemIdle, slSystemON, doShutdown, slMotorStop, kPublicEvent);

    // Define input actions for all levels
    slEmergency.setInputActions({               check(bMode, false, doRestart),     ignore(bPause)});
    slEmergencyShuttingDown.setInputActions({   ignore(bMode),                      ignore(bPause)});
    slEmergencyMotorStop.setInputActions({      ignore(bMode),                      ignore(bPause)});
    slShuttingDown.setInputActions({            ignore(bMode),                      check(bPause, false, doAbort)});
    slMotorStop.setInputActions({               ignore(bMode),                      check(bPause, false, doAbort)});
    slSystemOFF.setInputActions({               ignore(bMode),                      ignore(bPause)});
    slSystemIdle.setInputActions({              ignore(bMode),                      check(bPause, false, doAbort)});
    slSystemInit.setInputActions({              ignore(bMode),                      check(bPause, false, doAbort)});
    slMotorStart.setInputActions({              ignore(bMode),                      check(bPause, false, doAbort)});
    slSystemON.setInputActions({                ignore(bMode),                      check(bPause, false, doAbort)});

    // Define output actions for all levels
    slEmergency.setOutputActions({              set(ledRed, true),  set(ledGreen, false), set(led0, false)});
    slEmergencyShuttingDown.setOutputActions({  set(ledRed, true),  set(ledGreen, false), set(led0, false)});
    slEmergencyMotorStop.setOutputActions({     set(ledRed, true),  set(ledGreen, false), set(led0, false)});
    slSystemOFF.setOutputActions({              set(ledRed, false), set(ledGreen, false), set(led0, true)});
    slShuttingDown.setOutputActions({           set(ledRed, false), set(ledGreen, false), set(led0, true)});
    slMotorStop.setOutputActions({              set(ledRed, false), set(ledGreen, false), set(led0, true)});
    slSystemIdle.setOutputActions({             set(ledRed, false), set(ledGreen, true), set(led0, false)});
    slSystemInit.setOutputActions({             set(ledRed, false), set(ledGreen, true), set(led0, false)});
    slMotorStart.setOutputActions({             set(ledRed, false), set(ledGreen, true), set(led0, false)});
    slSystemON.setOutputActions({               set(ledRed, false), set(ledGreen, true), set(led0, false)});

    // Define and add level actions


    slEmergency.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
    });
    slEmergencyShuttingDown.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slEmergencyMotorStop.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slSystemOFF.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        eeros::Executor::stop();
    });
    slShuttingDown.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slMotorStop.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slSystemIdle.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        // no default next state
        // must be triggered from main
    });
    slSystemInit.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slMotorStart.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doNext);
    });
    slSystemON.setLevelAction([&](SafetyContext *privateContext) {
        // No default next safety level - this is the lowest safe level
        // only triggered by Abort or Shutdown
    });

    // Define entry level
    setEntryLevel(slSystemIdle);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doShutdown);
    });
}
