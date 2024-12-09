module top(
    input 		[7:0] in,					//输入:8位二进制
	input 		      en,					//使能端
    output reg 	[2:0] pos,  				//将输入进行高位优先编码得到3位二进制
	output reg 		  run,					//输入指示，若输入为非零，则输出1
	output reg 	[6:0] HEX0);				//将得到3位二进制转化为数码管译码

always@(*) 
begin
	if(en) 
	begin
		casez(in)
			8'b0000_0000: pos = 3'b000;
			8'b0000_0001: pos = 3'b000;
			8'b0000_001?: pos = 3'b001;
			8'b0000_01??: pos = 3'b010;
			8'b0000_1???: pos = 3'b011;
			8'b0001_????: pos = 3'b100;
			8'b001?_????: pos = 3'b101;
			8'b01??_????: pos = 3'b110;
			8'b1???_????: pos = 3'b111;
			default: pos = 3'b000;
		endcase
		end
	else
		pos=0;
end

always@(*) 
begin
	if(in==0)
		run=0;
	else
		run=1;
end

bcd7seg seg00( {1'b0,pos}, HEX0 );

endmodule

