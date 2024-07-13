  virtual function void write_ctrl(ctrl_packet trans);
    `uvm_info(get_type_name(), $sformatf("Reset: %0d",trans.data),UVM_FULL);
    if(trans.data == ACTIVE_RESET) begin
      stop_timer();
      expected_key = 0;
      expected_text = 0;
      counter = 0;
      S[0] = 4'hc; S[1] = 4'h5; S[2] = 4'h6; S[3] = 4'hb; 
      S[4] = 4'h9; S[5] = 4'h0; S[6] = 4'ha; S[7] = 4'hd; 
      S[8] = 4'h3; S[9] = 4'he; S[10] = 4'hf; S[11] = 4'h8; 
      S[12] = 4'h4; S[13] = 4'h7; S[14] = 4'h1; S[15] = 4'h2;
      nS[0] = 4'h5; nS[1] = 4'he; nS[2] = 4'hf; nS[3] = 4'h8; 
      nS[4] = 4'hc; nS[5] = 4'h1; nS[6] = 4'h2; nS[7] = 4'hd; 
      nS[8] = 4'hb; nS[9] = 4'h4; nS[10] = 4'h6; nS[11] = 4'h3; 
      nS[12] = 4'h0; nS[13] = 4'h7; nS[14] = 4'h9; nS[15]  = 4'ha;
    end
  endfunction : write_ctrl

  virtual function void write_cmd(cmd_packet trans);
    case(trans.data_type)
      KEY: begin
        stop_timer();
        expected_key = next_expected_key;        
        next_expected_key = trans.data;
        key_time = get_time();
        key_counter = get_time() + 31;
        counter =  get_time();
      end
      TEXT: begin
        if(get_time() <= key_counter) begin //done key update
          expected_text = trans.data;
          counter = key_counter + 32;
          start_timer(key_counter + 38);
          `uvm_info(get_type_name(), $sformatf("was start timer=%0d", get_time()), UVM_LOW);
        end else 
          if(get_time() > counter) begin
            expected_text = trans.data;
            counter = get_time() + 32;
            start_timer(get_time() + 38);
            `uvm_info(get_type_name(), $sformatf("start timer=%0d, counter=%0d", get_time(),counter), UVM_LOW);
          end else
            `uvm_info(get_type_name(), $sformatf("ignore packet time=%0d, counter=%0d", get_time(),counter), UVM_LOW);
      end
    endcase
  endfunction : write_cmd

  function void unupdate_word(inout logic [64-1:0] word, logic [64-1:0]key);
    aux = word;
    for (i = 0; i < 64; i++) begin
      word[i] = aux[(i%4)*16+i/4];
    end
    word[63:60]=nS[word[63:60]];
    word[59:56]=nS[word[59:56]];
    word[55:52]=nS[word[55:52]];
    word[51:48]=nS[word[51:48]];
    word[47:44]=nS[word[47:44]];
    word[43:40]=nS[word[43:40]];
    word[39:36]=nS[word[39:36]];
    word[35:32]=nS[word[35:32]];
    word[31:28]=nS[word[31:28]];
    word[27:24]=nS[word[27:24]];
    word[23:20]=nS[word[23:20]];
    word[19:16]=nS[word[19:16]];
    word[15:12]=nS[word[15:12]];
    word[11:8]=nS[word[11:8]];
    word[7:4]=nS[word[7:4]];
    word[3:0]=nS[word[3:0]];
    word = word ^ key;
  endfunction

  function void unupdate_key(inout logic[80-1:0]key,input logic[4:0] cnt);
    key[79:76] = nS[key[79:76]];
    key[19:15] = key[19:15] ^ cnt[4:0];
    aux = key;
    for (i = 79; i >= 0; i--) begin
      key[(i + 19) % 80] = aux[i];
    end
  endfunction

  function void update_key(inout logic[80-1:0]key,input logic[4:0] cnt);
    aux = key;
    for (i = 79; i >= 0; i--) begin
      key[i] = aux[(i + 19) % 80];
    end
    key[79:76] = S[key[79:76]];
    key[19:15] = key[19:15] ^ cnt[4:0];
  endfunction

  function void decript(inout logic[64-1:0] word, inout logic[80-1:0] key);
    for (cnt = 0; cnt < 31; cnt = cnt + 1) begin
      update_key(key, cnt + 1);
    end
    word = expected_text ^ key[79:16];
    saved_expected_key = key;

    for(cnt = 31; cnt > 0; cnt = cnt - 1) begin
      unupdate_key(key, cnt);
      unupdate_word(word, key[79:16]);
    end
    key= saved_expected_key;
  endfunction

  virtual function void write_out(out_packet trans);
    stop_timer();  
    expected_key = get_time() == key_time+1 ? expected_key : next_expected_key;

    `uvm_info(get_type_name(), $sformatf("text:=%0h, key:=%0h", expected_text, expected_key), UVM_MEDIUM);

    decript(expected_text, expected_key);

    if(get_time() == key_time +1) begin
      expected_key = next_expected_key;
      counter = get_time() +31;
    end
    else begin
      next_expected_key = expected_key;
    end

    `uvm_info(get_type_name(), $sformatf("text:=%0h, key:=%0h", expected_text, expected_key), UVM_MEDIUM);
    if(trans.out != expected_text) begin
      `uvm_info(get_type_name(), $sformatf("counter:=%0d, keycnt:=%0d", counter, key_counter), UVM_MEDIUM);
      `uvm_fatal(get_type_name(),
                 $sformatf("Output data mismatch.  Expected : %0h.  Actual : %0h", 
                           expected_text, trans.out))
      sbd_error = 1;
    end
  endfunction : write_out