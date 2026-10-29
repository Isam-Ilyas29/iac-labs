#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // Initialise top verilog instance
    Vsinegen *top = new Vsinegen;
    // Initialise trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("singen.vcd");

    // Initialise Vbuddy
    if (vbdOpen() != 1)
        return -1;
    vbdHeader("Lab 2: Task 1");

    // Initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 1;
    top->incr = 1;

    // Run simulation for many clock cycles
    for (i = 0; i < 1000000; ++i)
    {
        // Dump variables into VCD file + toggle clock
        for (clk = 0; clk < 2; ++clk)
        {
            tfp->dump(2 * i + clk); // Unit is in ps
            top->clk = !top->clk;
            top->eval();
        }

        // Need to set rst in first cycle as it is initially undefined (could be 0 or 1 when powered on)
        // The current counter has a synchronous reset. To implement asynchronous reset, you can change line 11 of counter.sv to detect change in rst signal. (See notes.)
        top->rst = (i < 2);

        top->offset = vbdValue();
        vbdPlot(top->dout1, 0, 255);
        vbdPlot(top->dout2, 0, 255);

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
