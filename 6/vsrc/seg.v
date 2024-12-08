module seg (
    input clk,
    input rst,
    input [3:0] num0,
    input [3:0] num1,
    output reg [6:0] seg0,
    output reg [6:0] seg1
);

wire [6:0] segs [15:0];
assign segs[0]  = 7'b0000001; // nvboard 0为发光 1为熄灭
assign segs[1]  = 7'b1001111;
assign segs[2]  = 7'b0010010;
assign segs[3]  = 7'b0000110;

assign segs[4]  = 7'b1001100;
assign segs[5]  = 7'b0100100;
assign segs[6]  = 7'b0100000;
assign segs[7]  = 7'b0001111;

assign segs[8]  = 7'b0000000;
assign segs[9]  = 7'b0000100;
assign segs[10] = 7'b0001000;
assign segs[11] = 7'b1100000;

assign segs[12] = 7'b0110001;
assign segs[13] = 7'b1000010;
assign segs[14] = 7'b0110000;
assign segs[15] = 7'b0111000;

always@(posedge clk)begin
    case (num0)
        4'd0 :seg0 = segs[0];
        4'd1 :seg0 = segs[1];
        4'd2 :seg0 = segs[2];
        4'd3 :seg0 = segs[3];
        4'd4 :seg0 = segs[4];
        4'd5 :seg0 = segs[5];
        4'd6 :seg0 = segs[6];
        4'd7 :seg0 = segs[7];
        4'd8 :seg0 = segs[8];
        4'd9 :seg0 = segs[9];
        4'd10:seg0 = segs[10];
        4'd11:seg0 = segs[11];
        4'd12:seg0 = segs[12];
        4'd13:seg0 = segs[13];
        4'd14:seg0 = segs[14];
        4'd15:seg0 = segs[15];
        default: seg0 = segs[0];
    endcase

    case (num1)
        4'd0 :seg1 = segs[0];
        4'd1 :seg1 = segs[1];
        4'd2 :seg1 = segs[2];
        4'd3 :seg1 = segs[3];
        4'd4 :seg1 = segs[4];
        4'd5 :seg1 = segs[5];
        4'd6 :seg1 = segs[6];
        4'd7 :seg1 = segs[7];
        4'd8 :seg1 = segs[8];
        4'd9 :seg1 = segs[9];
        4'd10:seg0 = segs[10];
        4'd11:seg0 = segs[11];
        4'd12:seg0 = segs[12];
        4'd13:seg0 = segs[13];
        4'd14:seg0 = segs[14];
        4'd15:seg0 = segs[15];
        default: seg1 = segs[0];
    endcase
end
endmodule
