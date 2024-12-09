module top(
    input       [1:0] d0,          	//输入: 四个2位二进制
    input  		[1:0] d1,
    input  		[1:0] d2,
    input  		[1:0] d3,
    input  		[1:0] Y,           	//输入: 控制
    
    output reg 	[1:0] F);      		//输出: 4选1

always @ (*) begin
    case(Y)
		3:  F=d3;
		2:  F=d2;
		1:  F=d1;
		0:  F=d0;
		default: F=d0;
    endcase
end
    
endmodule
