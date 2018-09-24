///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef PLUGINS_SAMPLESOURCE_BLADERF2INPUT_BLADERF2INPUTTHREAD_H_
#define PLUGINS_SAMPLESOURCE_BLADERF2INPUT_BLADERF2INPUTTHREAD_H_

// BladerRF2 is a SISO/MIMO device with a single stream supporting one or two Rx
// Therefore only one thread can be allocated for the Rx side
// All FIFOs must be registered before calling startWork() else SISO/MIMO switch will not work properly
// with unpredicatable results

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <libbladeRF.h>

#include "bladerf2/devicebladerf2shared.h"
#include "dsp/samplesinkfifo.h"
#include "dsp/decimators.h"

class Bladerf2InputThread : public QThread, public DeviceBladeRF2Shared::InputThreadInterface {
    Q_OBJECT

public:
    Bladerf2InputThread(struct bladerf* dev, unsigned int nbRxChannels, QObject* parent = NULL);
    virtual ~Bladerf2InputThread();

    virtual void startWork();
    virtual void stopWork();
    virtual bool isRunning() const { return m_running; }
    virtual unsigned int getNbChannels() const { return m_nbChannels; }
    virtual void setLog2Decimation(unsigned int channel, unsigned int log2_decim);
    virtual unsigned int getLog2Decimation(unsigned int channel) const;
    virtual void setFcPos(unsigned int channel, int fcPos);
    virtual int getFcPos(unsigned int channel) const;
    virtual void setFifo(unsigned int channel, SampleSinkFifo *sampleFifo);
    virtual SampleSinkFifo *getFifo(unsigned int channel);

private:
    struct Channel
    {
        SampleVector m_convertBuffer;
        SampleSinkFifo* m_sampleFifo;
        unsigned int m_log2Decim;
        int m_fcPos;
        Decimators<qint32, qint16, SDR_RX_SAMP_SZ, 12> m_decimators;

        Channel() :
            m_sampleFifo(0),
            m_log2Decim(0),
            m_fcPos(0)
        {}

        ~Channel()
        {
            if (m_sampleFifo) {
                delete[] m_sampleFifo;
            }
        }
    };

    QMutex m_startWaitMutex;
    QWaitCondition m_startWaiter;
    bool m_running;
    struct bladerf* m_dev;

    Channel *m_channels; //!< Array of channels dynamically allocated for the given number of Rx channels
    qint16 *m_buf; //!< Full buffer for SISO or MIMO operation
    unsigned int m_nbChannels;

    void run();
    unsigned int getNbFifos();
    void callbackSI(const qint16* buf, qint32 len, unsigned int channel = 0);
    void callbackMI(const qint16* buf, qint32 samplesPerChannel);
};



#endif /* PLUGINS_SAMPLESOURCE_BLADERF2INPUT_BLADERF2INPUTTHREAD_H_ */