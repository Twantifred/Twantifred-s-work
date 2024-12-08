module top(
    input      [3:0] A,			//操作数A(补码)
    input 	   [3:0] B,			//操作数B(补码)
    input 	   [2:0] ALUctr,	//功能选择:
								//000->A+B        001->A-B
								//010->Not A 	  011->A and B		  
								//100->A or B 	  101->A xor B
								//110->if(A<B)   111->if(A==B)

    output reg [3:0] ALUout,	//输出对应结果
    output reg [3:0] temp,		//缓存数据
    output reg 		 less,		//	
    output reg 		 of,		//溢出
	output reg 		 zf,		//Zero输出
	output reg 		 cf			//进位
);

reg [3:0] xb;

//initial 

always@(*)begin
	///
	begin 
	ALUout = 4'b0;
	temp = 4'b0; 
	less = 1'b0; 
	of = 0; 
	zf = 0; 
	cf = 0;  
	xb = 4'b0;
	end
	///
  	casez(ALUctr)
		3'b00z:	//   A+B/A-B
			begin 
			xb = B^{ {4{ALUctr[0]}} };					
			{cf,ALUout} = xb + A + {3'b0,ALUctr[0]};	   //若为减法,则需要对B取反
			of = (A[3] == xb[3]) && (ALUout[3] != A[3]);   //判断是否溢出
			zf = ~(|ALUout);							   //判断是否为0
			end

		3'b010:	//   Not A
			ALUout = ~A;

		3'b011:begin // and
			ALUout = A & B;
			end

		3'b100:begin // or
			ALUout = A | B;
			end

		3'b101:begin // xor
			ALUout = A ^ B;
			end

		3'b110:begin // if(A<B)
			xb = B ^ 4'b1111;  // 用减法进行比较大小
			{cf,temp} = xb + A + 4'b1;
			of = (A[3] == xb[3]) && (temp[3] != A[3]);
			zf = ~(|temp);
			less = temp[3] ~^ of;
			if(less == 0) 
				ALUout = 4'b1;
			else 
				ALUout = 4'b0;
			end

		3'b111:begin // if(A==B) 根据zf判断
			xb = B ^ 4'b1111;
			{cf,temp} = xb + A + 4'b1;
			of = (A[3] == xb[3]) && (temp[3] != A[3]);
			zf = ~(|temp);
			if(zf == 1) 
				ALUout = 4'b1;
			else 
				ALUout = 4'b0;
			end

		default:
			begin 
				ALUout = 4'b0;
				temp = 4'b0; 
				less = 1'b0; 
				of = 0; 
				zf = 0; 
				cf = 0;  
				xb = 4'b0;
			end
  endcase

end

endmodule
