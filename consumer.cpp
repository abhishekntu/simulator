
#ifndef _CONSUMER_
#define _CONSUMER_

#include <iostream>

#include "systemc.h"

/*
 * Consumer waits for data appearing on its bus, and
 * echoes such data.
 */
class Consumer : public sc_module {
    public:
        sc_in<int> value;       // data port

        SC_CTOR(Consumer) {
            SC_THREAD(consume);
            // make "consume()" sensible to changes on the value port
            sensitive << value;
        }

        // module activity function
        void consume() {
            int x;
            while (true) {
                // wait for new data to appear on the value port
                wait();
                // fetch it and display its val
                x = value.read();
                cout << x << endl;
            }
        }
};

#endif

