module top_(
    input clk,
    input rst,
    output reg [7:0] num,
    output reg [3:0] num0,
    output reg [3:0] num1,
    output reg [6:0] seg0,
    output reg [6:0] seg1
);

reg x=0;
always@(posedge clk)begin
    if(rst)  
        num = 8'b00000001;
    else begin
        x = num[4] ^ num[3] ^ num[2] ^ num[0]; // 串行阻塞赋值
        num = {x,num[7:1]}; 
    end
end

assign num0 = num[3:0];
assign num1 = num[7:4];

seg my_seg(
  .clk      (clk),
  .rst      (rst),
  .num0     (num0),
  .num1     (num1),
  .seg0     (seg0),
  .seg1     (seg1)
);

endmodule
