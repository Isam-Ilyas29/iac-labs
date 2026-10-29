module sigdelay #(
    parameter ADDRESS_WIDTH = 9,
              DATA_WIDTH    = 8
) (
    input logic                       clk,
    input logic                       rst,
    input logic                       wr,
    input logic                       rd,
    input logic [ADDRESS_WIDTH-1 : 0] offset,
    input logic [   DATA_WIDTH-1 : 0] mic_signal,

    output logic [DATA_WIDTH-1 : 0] delayed_signal
);

    logic [ADDRESS_WIDTH-1 : 0] addr;

    counter #(
        .WIDTH(ADDRESS_WIDTH)
    ) my_counter (
        .clk  (clk),
        .rst  (rst),
        .en   (1'b1),
        .incr (ADDRESS_WIDTH'(1)),
        .count(addr)
    );

    ram2ports #(
        .ADDRESS_WIDTH(ADDRESS_WIDTH),
        .DATA_WIDTH   (DATA_WIDTH)
    ) my_ram (
        .clk    (clk),
        .wr_en  (wr),
        .rd_en  (rd),
        .wr_addr(addr),
        .rd_addr(addr - offset),
        .din    (mic_signal),
        .dout   (delayed_signal)
    );

endmodule
