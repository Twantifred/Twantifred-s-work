module top (                    //随机数发生器
    input wire       clk,       //这是时钟
    input wire [7:0] din,       //输入数据
    input wire [1:0] shamt,     //模式选择
								//00->复位           01->置数
								//10->主要功能 	      11->保持		  

    output reg [7:0] data       //输出数据
);
    
    
    always @(posedge clk) begin
        if (shamt == 2'b00)         //00->复位
            data <= 8'b0;
        else if (shamt == 2'b01)    //01->置数
            data <= din;
        else if (shamt == 2'b10)    //10->主要功能 具体见讲义
            data <= {data[4] ^ data[3] ^ data[2] ^ data[0], data[7:1]};
        else                        //11->保持
            data <= data;
    end

endmodule