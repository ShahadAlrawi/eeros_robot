#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    :   q1("quat1"),
        servo1("servo1"),
        gain(2.0),
        timedomain("Main time domain", dt, true) // realtime = true
{
    // Name all blocks
    // input from IMU (quaternion 1)
    q1.setName("q1");
    // output to the servo
    servo1.setName("servo1");
    // gain
    gain.setName("My gain");

    // Name all signals
    q1.getOut().getSignal().setName("Alpha/2"); // output signal name of q1 peripheral input
    gain.getOut().getSignal().setName("Alpha"); // output signal name of gain

    // Connect signals
    gain.getIn().connect(q1.getOut());
    servo1.getIn().connect(gain.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(q1);
    timedomain.addBlock(gain);
    timedomain.addBlock(servo1);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}