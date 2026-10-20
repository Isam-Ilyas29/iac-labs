module counter #(
  parameter WIDTH = 8
)(
  // Interface signals
  input  logic             clk,     // Clock
  input  logic             rst,     // Reset
  input  logic             ld,      // Load counter from data
  input  logic [WIDTH-1:0] v,       // Value to preload
  output logic [WIDTH-1:0] count    // Count output
);

always_ff @ (posedge clk)
  if (rst) count <= {WIDTH{1'b0}};
  else     count <= ld ? v : count + {{WIDTH-1{1'b0}},1'b1};
  // Single stepping
  // else     count <= count + {{WIDTH-1{1'b0}},ld};
endmodule
