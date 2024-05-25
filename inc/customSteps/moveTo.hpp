#ifndef MOVETO_HPP_
#define MOVETO_HPP_

#include <eeros/sequencer/Step.hpp>
#include "ControlSystem.hpp"

class MoveTo : public eeros::sequencer::Step
{
public:
    MoveTo(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : eeros::sequencer::Step(name, caller), cs(cs)
    {
        log.info() << "Step created: " << name;
    }

    int operator() (double c)
    {
        this->c = c;
        return start();
    }

    int action()
    {
        log.info() << "Moving to " <<c<<" rad.";
        cs.c.setValue(c);
        return 0;
    }

private:
    // Define variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
    double c;

};

#endif // MOVETO_HPP_
