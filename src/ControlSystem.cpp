#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    :   q1("quat1"),
        servo1("servo1"),
        gain(2.0),
        c(0.0),
        timedomain("Main time domain", dt, true) // realtime = true
{
    /* BLOCK NAMES START HERE */
    // input from IMU (quaternion 1)
    q1.setName("q1");
    // output to the servo
    servo1.setName("servo1");
    // gain
    gain.setName("My gain");
    // constant
    c.setName("Servo setpoint");
    /* BLOCK NAMES END HERE */



    /* SIGNAL OUPUT NAMES START HERE */
    q1.getOut().getSignal().setName("Alpha/2"); // output signal name of q1 peripheral input
    gain.getOut().getSignal().setName("Alpha"); // output signal name of gain
    c.getOut().getSignal().setName("Servo Setpoint [rad]");
    /* SIGNAL OUPUT NAMES END HERE */



    /* CONNECT START HERE*/
    // currently the gain is fed from q1 sensor but not used
    gain.getIn().connect(q1.getOut());

    // connect the constant to the servo
    servo1.getIn().connect(c.getOut());
    /* CONNECT END HERE*/

    // Add blocks to timedomain
    timedomain.addBlock(q1);
    timedomain.addBlock(gain);
    timedomain.addBlock(c);
    timedomain.addBlock(servo1);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}