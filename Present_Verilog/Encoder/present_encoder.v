`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2024 10:39:45 AM
// Design Name: 
// Module Name: present_encoder
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
`define INITIAL_KEY 0
`define NR_ROUNDS 32

module present_encoder(clk,n_reset,pl,in_text,ciphertext,done);
  input clk,
  		n_reset;
  input [1:0] pl;
  input	[79:0]in_text;
  output wire done;
  output [63:0]ciphertext;

  reg[63:0] word;
  reg[79:0] key;
  reg[5:0] 	counter;
  reg 		reseted;
  reg[3:0] 	S[0:15];
    
  wire[63:0] x;
  wire[63:0] s;
  wire[63:0] p;
  
  assign x	= word ^ key[79:16];
  assign s	= {S[x[63:60]],
               S[x[59:56]],
               S[x[55:52]],
               S[x[51:48]],
               S[x[47:44]],
               S[x[43:40]],
               S[x[39:36]],
               S[x[35:32]],
               S[x[31:28]],
               S[x[27:24]],
               S[x[23:20]],
               S[x[19:16]],
               S[x[15:12]],
               S[x[11:8]],
               S[x[7:4]],
               S[x[3:0]]};
  assign p = {s[63],s[59],s[55],s[51],s[47],s[43],s[39],s[35],s[31],s[27],s[23],s[19],s[15],s[11],s[7],s[3],
              s[62],s[58],s[54],s[50],s[46],s[42],s[38],s[34],s[30],s[26],s[22],s[18],s[14],s[10],s[6],s[2],
              s[61],s[57],s[53],s[49],s[45],s[41],s[37],s[33],s[29],s[25],s[21],s[17],s[13],s[9] ,s[5],s[1],
              s[60],s[56],s[52],s[48],s[44],s[40],s[36],s[32],s[28],s[24],s[20],s[16],s[12],s[8] ,s[4],s[0]};
  assign done 		= (counter == `NR_ROUNDS);
  assign ciphertext	= done ? ( word ^ key[79:16] ) : 64'hz;

  always@(posedge clk or negedge n_reset)
    if(!n_reset)
      begin
        counter	<= 1;
        word	<= 0;
        reseted	<= 1;
        key		<= `INITIAL_KEY;
//        S<=64'hc56b90ad3ef84712;
        $readmemh("smem.mem", S);
      end
  else
    if(reseted || done)
      begin
        if(pl[1])
        begin
            key <= in_text;
        end
        if(pl[0])
          begin
            reseted	<= 0;
            counter	<= 1;
            word	<= in_text[63:0];
          end
      end
  else
    if(!done)
      begin
        counter	<= counter+1;
        key		<={S[key[18:15]],
                   key[14:0],key[79:39],
                   key[38:34]^counter[4:0],
                   key[33:19]
                  };
        word	<= p;
      end
endmodule

