#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // Initialise top verilog instance
    Vtop *top = new Vtop;
    // Initialise trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("top.vcd");

    // Initialise Vbuddy
    if (vbdOpen() != 1)
        return -1;
    vbdHeader("Lab 1: BCD Counter");

    // Initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // Run simulation for many clock cycles
    for (i = 0; i < 1000; ++i)
    {
        // Dump variables into VCD file + toggle clock
        for (clk = 0; clk < 2; ++clk)
        {
            tfp->dump(2 * i + clk); // Unit is in ps
            top->clk = !top->clk;
            top->eval();
        }

        // Send BCD digits to Vbuddy (hundreds, tens, units)
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);
        vbdCycle(i + 1);

        // Inputs for next cycle
        top->rst = (i < 2);
        top->en = vbdFlag(); // Press knob to start/stop counting

        if (Verilated::gotFinish())
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
