#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include <eeros/sequencer/Wait.hpp>
#include "ControlSystem.hpp"
#include "customSteps/moveTo.hpp"
#include "customSequences/orientationException.hpp"
#include <eeros/sequencer/Monitor.hpp>

class MainSequence : public eeros::sequencer::Sequence
{
public:
    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 MyRobotSafetyProperties &sp, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),
          sleep("Sleep", this),
          moveTo("moveTo", this, cs),
          checkOrientation(0.1, cs),
          orientationException("orientaion exception", this, cs, checkOrientation),
          orienationMonitor("orientation monitor", this, checkOrientation, eeros::sequencer::SequenceProp::resume, &orientationException)
    {
        addMonitor(&orienationMonitor);
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running)
        {
            moveTo(-0.5);
            sleep(1.0);
            moveTo(0.5);
            sleep(1.0);
        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;

    eeros::sequencer::Wait sleep;
    MoveTo moveTo;
    CheckOrientation checkOrientation;
    OrientationException orientationException;
    eeros::sequencer::Monitor orienationMonitor;

};

#endif // MAINSEQUENCE_HPP_