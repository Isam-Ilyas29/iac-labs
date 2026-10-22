#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // Initialise top verilog instance
    Vcounter *top = new Vcounter;
    // Initialise trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Initialise Vbuddy
    if (vbdOpen() != 1)
        return -1;
    vbdHeader("Lab 1: Counter");

    // Initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    int n = 0;

    // Run simulation for many clock cycles
    for (i = 0; i < 300; ++i)
    {
        // Dump variables into VCD file + toggle clock
        for (clk = 0; clk < 2; ++clk)
        {
            tfp->dump(2 * i + clk); // Unit is in ps
            top->clk = !top->clk;
            top->eval();
        }

        // Send count to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i + 1);

        // Need to set rst in first cycle as it is initially undefined (could be 0 or 1 when powered on)
        // The current counter has a synchronous reset. To implement asynchronous reset, you can change line 11 of counter.sv to detect change in rst signal. (See notes.)
        top->rst = i < 2;
        top->en = vbdFlag();

        // Modify the testbench so that you stop counting for 3 cycles once the counter reaches 0x9, and then resume counting. You may also need to change the stimulus for rst.
        // if (top->count == 9 && n++ < 3)
        // {
        //     top->en = 0;
        // }
        // else
        // {
        //     top->en = 1;
        // }

        if (Verilated::gotFinish())
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
