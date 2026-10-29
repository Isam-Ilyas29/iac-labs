module sinegen #(
    parameter WIDTH = 8
) (
    input logic               clk,
    input logic               rst,
    input logic               en,
    input logic [WIDTH-1 : 0] incr,

    output logic [WIDTH-1 : 0] dout
);

    logic [WIDTH-1 : 0] addr;

    counter #(
        .WIDTH(WIDTH)
    ) my_counter (
        .clk  (clk),
        .rst  (rst),
        .en   (en),
        .incr (incr),
        .count(addr)
    );

    rom #(
        .ADDRESS_WIDTH(WIDTH),
        .DATA_WIDTH(WIDTH)
    ) my_rom (
        .clk (clk),
        .addr(addr),
        .dout(dout)
    );

endmodule
