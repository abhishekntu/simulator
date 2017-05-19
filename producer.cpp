
#ifndef _PRODUCER_
#define _PRODUCER_

#include "systemc.h"

/*
 * This module generates data on every clock cycle,
 * with an incremental basis, for all of its lifetime.
 */
class Producer : public sc_module {
    private:
        int cnt;

    public:
        sc_out<int> value;  // data port
        sc_in<bool> clk;    // clock port

        SC_CTOR(Producer) {
            cnt = 0;
            SC_THREAD(produce);
            // make the "produce" thread sensitive to the positive clock level
            sensitive << clk.pos();
        }

        void produce() {
            while (true) {  // do forever:
                wait();     // wait the beginning of a clock cycle
                // possibly update the counter
                if (++cnt == 1000)
                    cnt = 0;
                // write data
                value.write(cnt);
            }
        }
};

#endif

