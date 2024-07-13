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
  reg[63:0] word, next_word;
  reg was_pl;
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
    if(!n_reset) begin
      key<=`INITIAL_KEY;
      skey<=`INITIAL_KEY;
      word<=0;
      next_word <= 0;
      was_pl <= 0;
      counter<=`NR_ROUNDS;
      key_counter<=1;
      start<=0;
      reseted<=1;
      S[0] <= 4'hc; S[1] <= 4'h5; S[2] <= 4'h6; S[3] <= 4'hb; 
      S[4] <= 4'h9; S[5] <= 4'h0; S[6] <= 4'ha; S[7] <= 4'hd; 
      S[8] <= 4'h3; S[9] <= 4'he; S[10] <= 4'hf; S[11] <= 4'h8; 
      S[12] <= 4'h4; S[13] <= 4'h7; S[14] <= 4'h1; S[15] <= 4'h2;
      //        $readmemh("smem.mem", S);
      nS[0] <= 4'h5; nS[1] <= 4'he; nS[2] <= 4'hf; nS[3] <= 4'h8; 
      nS[4] <= 4'hc; nS[5] <= 4'h1; nS[6] <= 4'h2; nS[7] <= 4'hd; 
      nS[8] <= 4'hb; nS[9] <= 4'h4; nS[10] <= 4'h6; nS[11] <= 4'h3; 
      nS[12] <= 4'h0; nS[13] <= 4'h7; nS[14] <= 4'h9; nS[15] <= 4'ha;
      //        $readmemh("nsmem.mem", nS);
    end
  else begin
    if((!ready || !start) && !pl[1])  begin
      if(key_counter+1==`NR_ROUNDS)
        start<=1;
      key_counter<=key_counter+1;
      skey<={S[skey[18:15]],
             skey[14:0],skey[79:39],
             skey[38:34]^key_counter[4:0],
             skey[33:19]};
    end
    
    if(pl[1]) begin
      start <= 0;
      reseted <= 1;
      key_counter<=1;
      skey <= in_text;
      was_pl<=0;
    end
    
    if(!start && pl[0] && (reseted || done)) begin
      next_word <= in_text[63:0];
      was_pl <= 1;
    end

    if(start && (reseted || done)) begin
      if(pl[0] || was_pl) begin
        reseted<=0;
        key_counter<=1;
        counter<=`NR_ROUNDS;
        key<={skey[60:20],
              skey[19:15]^(key_counter[4:0]-5'h1),
              skey[14:0],
              nS[skey[79:76]],
              skey[75:61]};
        word<=(was_pl ? next_word : in_text[63:0])^skey[79:16];
        was_pl <= 0;
      end
    end

    if(start && !done && !reseted) begin
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