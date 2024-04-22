`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/21/2024 10:24:32 PM
// Design Name: 
// Module Name: present_decoder
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

module present_decoder(clk,n_reset,pl,text,in_text,done);
  input clk,n_reset;
  input [1:0] pl;
  input[79:0] in_text;
  output done;
  output[63:0] text;

  reg[5:0] counter;
  reg[5:0] key_counter;
  reg[63:0] word;
  reg[79:0] key;
  reg[79:0] skey;
  reg[3:0] S[0:15];
  reg[3:0] nS[0:15];
  reg reseted;
  reg start;
  wire ready;

  assign done= counter==1;
  assign ready= key_counter==`NR_ROUNDS;
  assign text=done?word:64'hz;

  always@(negedge clk or negedge n_reset)
    if(!n_reset)
      begin
        key<=`INITIAL_KEY;
        skey<=`INITIAL_KEY;
        word<=0;
        counter<=`NR_ROUNDS;
        key_counter<=1;
        start<=0;
        reseted<=1;
        $readmemh("smem.mem", S);
        $readmemh("nsmem.mem", nS);
      end
  else
    begin
      if((!ready || !start) && !pl[1])
        begin
          if(key_counter+1==`NR_ROUNDS)
            start<=1;
          key_counter<=key_counter+1;
          skey<={S[skey[18:15]],
                 skey[14:0],skey[79:39],
                 skey[38:34]^key_counter[4:0],
                 skey[33:19]};
        end
      if(pl[1])
      begin
        start <= 0;
        reseted <= 1;
        key_counter<=1;
        skey <= in_text;
      end
      
      if(start && (reseted || done))
        begin
          if(pl[0])
            begin
              reseted<=0;
              key_counter<=1;
              counter<=`NR_ROUNDS;
              key<={skey[60:20],
                    skey[19:15]^(key_counter[4:0]-5'h1),
                    skey[14:0],
                    nS[skey[79:76]],
                    skey[75:61]};
              word<=in_text[63:0]^skey[79:16];
            end
        end

      if(start && !done && !reseted)
        begin
          counter<=counter-1;
          key<={key[60:20],
                key[19:15]^(counter[4:0]-5'h2),
                key[14:0],
                nS[key[79:76]],
                key[75:61]};
          word<={nS[{word[63],word[47],word[31],word[15]}],
                 nS[{word[62],word[46],word[30],word[14]}],
                 nS[{word[61],word[45],word[29],word[13]}],
                 nS[{word[60],word[44],word[28],word[12]}],
                 nS[{word[59],word[43],word[27],word[11]}],
                 nS[{word[58],word[42],word[26],word[10]}],
                 nS[{word[57],word[41],word[25],word[9]}],
                 nS[{word[56],word[40],word[24],word[8]}],
                 nS[{word[55],word[39],word[23],word[7]}],
                 nS[{word[54],word[38],word[22],word[6]}],
                 nS[{word[53],word[37],word[21],word[5]}],
                 nS[{word[52],word[36],word[20],word[4]}],
                 nS[{word[51],word[35],word[19],word[3]}],
                 nS[{word[50],word[34],word[18],word[2]}],
                 nS[{word[49],word[33],word[17],word[1]}],
                 nS[{word[48],word[32],word[16],word[0]}]}^key[79:16];
        end
    end
endmodule