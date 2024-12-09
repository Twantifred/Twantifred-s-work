module barrel_shifter(          //桶形移位器
    input      [7:0] din,       //输入数据
    input      [2:0] shamt,     //移位位数
    input            LR,        //选择端L/R表示左移和右移，置为1为左移，置为0为右
    input            AL,        //选择端A/L为算术逻辑选择，置为1为算术移位，置为0为逻辑移位。

    output reg [7:0] dout       //输出数据
);

always @(*) begin
    if(LR == 1) begin                                //选择端L/R置为1,左移,此时算术位移和逻辑位移一样
        dout = shamt[2] ? {din[3:0], 4'b0} : din;           //移动4位
        dout = shamt[1] ? {dout[5:0], 2'b0} : dout;         //移动2位
        dout = shamt[0] ? {dout[6:0], 1'b0} : dout;         //移动1位
    end 

    else begin                  //选择端L/R置为0,右移,此时算术位移和逻辑位移并非一样
        if(AL == 1)begin        //选择端A/L置为1,算术移位，
            dout = shamt[2] ? {{ 4{din[7]}}, din[7:4]} : din;
            dout = shamt[1] ? {{ 2{dout[7]}}, dout[7:2]} : dout;
            dout = shamt[0] ? {{ 1{dout[7]}}, dout[7:1]} : dout;
        end
        else begin
            dout = shamt[2] ? {4'b0, din[7:4]} : din;
            dout = shamt[1] ? {2'b0, dout[7:2]} : dout;
            dout = shamt[0] ? {1'b0, dout[7:1]} : dout;      
        end
    end
end

endmodule
