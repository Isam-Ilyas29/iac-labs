module sinegen #(
    parameter WIDTH = 8
) (
    input logic               clk,
    input logic               rst,
    input logic               en,
    input logic [WIDTH-1 : 0] incr,
    input logic [WIDTH-1 : 0] offset,

    output logic [WIDTH-1 : 0] dout1,
    output logic [WIDTH-1 : 0] dout2
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
        .clk  (clk),
        .addr1(addr),
        .addr2(addr + offset),
        .dout1(dout1),
        .dout2(dout2)
    );

endmodule
