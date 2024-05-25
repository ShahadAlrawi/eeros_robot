#ifndef ORIENTATIONEXCEPTION_HPP_
#define ORIENTATIONEXCEPTION_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Condition.hpp>
#include "ControlSystem.hpp"

class CheckOrientation : public eeros::sequencer::Condition
{
public:
    CheckOrientation(double angle, ControlSystem &cs) : angle(angle), cs(cs) {}
    bool validate() {
        return abs(cs.gain.getOut().getSignal().getValue()) > angle;
    }

private:
    ControlSystem &cs;
    double angle;
};

/*
    This sequence is acting as exception handling
    While it is defined as a sequence with an exit condition
    It will move to state "running" when check is true and only when check is false
    it exist from the exception
*/
class OrientationException : public eeros::sequencer::Sequence
{
public:
    OrientationException(std::string name, eeros::sequencer::Sequence *caller,
                        ControlSystem &cs, CheckOrientation check)
        : eeros::sequencer::Sequence(name, caller, true),
        cs(cs),
        check(check)

    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        log.warn() << "Orientation around x is outside of the allowed range!";
        return 0;
    }

    /* Exit Conditon STARTS HERE */
    bool checkExitCondition()
    {
        return !check.validate();
    }
    /* Exit Conditon ENDS HERE */

private:
    ControlSystem &cs;
    CheckOrientation check;

};

#endif // ORIENTATIONEXCEPTION_HPP_
