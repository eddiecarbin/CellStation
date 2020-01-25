#ifndef PANELLIGHTCONTROLLER_H_
#define PANELLIGHTCONTROLLER_H_

#include <Arduino.h>

enum PanelStateEnum{
    OFF,
    ON
};

class PanelLightController
{
public:
    PanelLightController(int pin);
    PanelStateEnum getState(void);
    void setState( PanelStateEnum data );
    void initialize();
    void update(void);
    virtual ~PanelLightController();

private:
    int pin;
    PanelStateEnum currentState;
};

#endif /* PANELLIGHTCONTROLLER_H_ */