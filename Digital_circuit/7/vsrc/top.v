module top (
	input 		 clk,
	input 		 clrn,
	input 		 ps2_clk,
	input		 ps2_data,
	output [7:0] seg0,
	output [7:0] seg1,
	output [7:0] seg2,
	output [7:0] seg3,
	output [7:0] seg4,
	output [7:0] seg5,
	output [7:0] seg6,
	output [7:0] seg7,

	output  reg [4:0] Test
);

	assign seg6=8'b11111111;
	assign seg7=8'b11111111;

	reg nextdata_n;
	reg ready;
	reg overflow;
	reg [7:0] data,tmp_data,out_data;
	reg [7:0] key_count;
	reg [7:0] ascii;

	reg [7:0] tmp_seg0,tmp_seg1,tmp_seg2,tmp_seg3;
	
	//四个状态
	reg [3:0] a=4'b0000;//初始状态，等待输入
	reg [3:0] b=4'b0001;//这个状态，是out_data写入
	reg [3:0] c=4'b0010;//将out_data的数据显示在数码管上
	reg [3:0] d=4'b0100;//等待下一次输入

	reg [3:0] state=a;
	reg [3:0] next_state;
	//输出ready，overflow，tmp_data，key_count
	ps2_keyboard my_keyboard (clk,clrn,ps2_clk,ps2_data,tmp_data,ready,nextdata_n,overflow,key_count);
	ps2_to_ascii my_ps2_to_ascii (data,ascii);//键码转化为ascii

	bcd7seg my_seg1 (data[3:0],tmp_seg0);
	bcd7seg my_seg2 (data[7:4],tmp_seg1);
	bcd7seg my_seg3 (ascii[3:0],tmp_seg2);
	bcd7seg my_seg4 (ascii[7:4],tmp_seg3);
	bcd7seg my_seg5 (key_count[3:0],seg4);
	bcd7seg my_seg6 (key_count[7:4],seg5);

	//触发器更新seg
	Reg #(8,8'b11111111) my_reg1 (clk,state!=b,tmp_seg0,seg0,state==b); //初始状态
	Reg #(8,8'b11111111) my_reg2 (clk,state!=b,tmp_seg1,seg1,state==b); //out_data写入
	Reg #(8,8'b11111111) my_reg3 (clk,state!=b,tmp_seg2,seg2,state==b); //数据显示在数码管
	Reg #(8,8'b11111111) my_reg4 (clk,state!=b,tmp_seg3,seg3,state==b); //等待下一次输入，直接进入状态b

  
	always @(posedge clk or ps2_clk) begin//更新数据
	if (clrn) state<=a;
	else  state<=next_state;
	end

	always @(*) begin//状态机改变
		if(ready) begin
			case (state)
			a:next_state=b;
			b:next_state=(tmp_data==8'hf0)?c:b;//tmp_data==8'hf0表示松开了按键
			c:next_state=(tmp_data!=8'hf0)?d:c;//状态不为0的时候尽快到状态b更新数据
			d:next_state=(tmp_data!=8'hf0)?b:d;
			default:next_state=a;
			endcase
			out_data=tmp_data;
		end
	end

	always @(*) begin
		case(state)
			a:Test={ready,4'b0001};
			b:Test={ready,4'b0010};
			c:Test={ready,4'b0100};
			d:Test={ready,4'b1000};
			default:Test={ready,4'b0000};
		endcase
	end

	assign data=(state==b)?out_data:8'b0;

endmodule

