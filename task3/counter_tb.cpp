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
    vbdSetMode(1);

    // Initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;

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

        top->rst = i < 2;
        top->ld = vbdFlag();
        top->v = vbdValue();

        if (Verilated::gotFinish())
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
