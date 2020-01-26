#ifndef SOUNDPLAYER_H_
#define SOUNDPLAYER_H_
/*
 * SoundPlayer.h
 *
 *  Created on: Jan 19, 2016
 *      Author: carbi
 */

#include "DFRobotDFPlayerMini.h"

class SoundPlayer
{

    static SoundPlayer *s_instance;
    SoundPlayer(int v = 0)
    {
        // m_value = v;
    }

public:
    SoundPlayer(int pin, int tollerance);
    void initialize(void);
    void volume(uint8_t volume);
    bool isPlaying();
    void PlaySound(int value = 1);
    void StopSound(void);
    void update(void);
    virtual ~SoundPlayer();

    static SoundPlayer *instance()
    {
        if (!s_instance)
            s_instance = new SoundPlayer;
        return s_instance;
    }

private:
    int pin;
    int fileCount;
    bool soundPlaying;
    void printDetail(uint8_t type, int value);
    DFRobotDFPlayerMini myDFPlayer;
};

#endif /* SOUNDPLAYER_H_ */