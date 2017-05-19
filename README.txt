
########################
        PRODCONS
########################

last update aug 18, 2006
report bugs to <mij@bitchx.it>


    @ Outline @

This example introduces some more tools for modelling modules with signals in
SystemC.

A producer generates data on a bus with a fixed frequency regulated by a
system clock. A consumer reacts to changes on its bus fetching data and
echoing it to output.

While modules are instructed to maintain their behaviour endlessly, the
simulation is forcedly interrupted after 50 clock cycles.

The usage of clocks is introduced. Clocks are used to specify time-discrete
systems. They are represented like bool signals and their properties are
defined on construction time, notably period, duty cycle and time unit
measure.


    @ Goals @

This example provides a more consistent introduction to:
- clocks
- change events & sensitivity lists
- time-controlled simulation


    @ Compile & Run @

$ make
$ ./prodcons

The expected output is one of an empty SystemC simulation:

$ ./prodcons

             SystemC 2.1.v1 --- Aug 16 2006 17:49:45
        Copyright (c) 1996-2005 by all Contributors
                    ALL RIGHTS RESERVED
1
2
3
4
5
$

