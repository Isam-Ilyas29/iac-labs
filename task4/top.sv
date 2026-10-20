module top #(
  parameter WIDTH = 8
)(
  // Interface signals
  input  logic            clk,      // Clock
  input  logic            rst,      // Reset
  input  logic            en,       // Enable
  output logic [11:0]     bcd       // Count output as 3 BCD digits
);

  logic  [WIDTH-1:0]      count;    // Interconnect wire

counter myCounter (
  .clk (clk),
  .rst (rst),
  .en (en),
  .count (count)
);

bin2bcd myDecoder (
  .x (count),
  .BCD (bcd)
);

endmodule
