`uvm_analysis_imp_decl(_ctrl)
`uvm_analysis_imp_decl(_cmd)
`uvm_analysis_imp_decl(_out)

class scoreboard extends uvm_scoreboard;

  uvm_analysis_imp_ctrl#(ctrl_packet, scoreboard) ctrl_item_collected_export;
  uvm_analysis_imp_cmd#(cmd_packet, scoreboard)   cmd_item_collected_export;
  uvm_analysis_imp_out#(out_packet, scoreboard)   out_item_collected_export;

  protected virtual cmd_if cmd_vif;
  protected integer current_time = 0;
  //watchdog trigger time register
  protected integer trigger_time = 0;
  //watchdog start register
  protected bit start = 0;

  protected bit disable_scoreboard = 0;
  int sbd_error = 0;

  //values of reset and oe signals
  bit reset = 0;

  logic [80-1:0] expected_key;
  logic [64-1:0] expected_text;
  logic [5:0] counter = 0;
  
  logic [3:0] S[15:0];
  logic [80-1:0] aux;
  int i;
  int cnt;

  `uvm_component_utils_begin(scoreboard)
  `uvm_field_int(disable_scoreboard, UVM_DEFAULT)
  `uvm_component_utils_end

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  //build_phase
  function void build_phase(uvm_phase phase);
    ctrl_item_collected_export = new("ctrl_item_collected_export", this);
    cmd_item_collected_export =  new("cmd_item_collected_export", this);
    out_item_collected_export =  new("out_item_collected_export", this);

    if(!uvm_config_db#(virtual cmd_if)::get(this, "", "cmd_vif", cmd_vif))
      `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".cmd_vif"});
  endfunction : build_phase

  virtual task run_phase(uvm_phase phase);
	fork
      //for every clk period increment current_time
      forever begin
        @(negedge cmd_vif.clk);
        current_time = current_time + 1;
        if(counter) begin
          counter = counter + 1;
        end
      end
      // when time reaches trigger stop test
      forever begin
        @(current_time == trigger_time iff start);
        `uvm_info(get_type_name(), $sformatf("expected_key=%0d, \texpected_text=%0d",
                                             expected_key, expected_text), UVM_LOW);
        `uvm_info(get_type_name(), $sformatf("time=%0d", get_time()), UVM_LOW);
        `uvm_info(get_type_name(), "Watchdog trigger",UVM_LOW);
      end
    join
  endtask : run_phase

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
    end
  endfunction : write_ctrl

  virtual function void write_cmd(cmd_packet trans);
    if(counter == 0) begin
      case(trans.data_type)
        KEY: begin
          expected_key = trans.data;
        end
        TEXT: begin
		  expected_text = trans.data;
          counter = 1;
          start_timer(current_time + 38);
          `uvm_info(get_type_name(), $sformatf("start timer=%0d", get_time()), UVM_LOW);
        end
      endcase
    end
  endfunction : write_cmd
  
  function void update_word(inout logic [64-1:0] word, logic [64-1:0]key);
    word = word ^ key;
    word[63:60]=S[word[63:60]];
    word[59:56]=S[word[59:56]];
    word[55:52]=S[word[55:52]];
    word[51:48]=S[word[51:48]];
    word[47:44]=S[word[47:44]];
    word[43:40]=S[word[43:40]];
    word[39:36]=S[word[39:36]];
    word[35:32]=S[word[35:32]];
    word[31:28]=S[word[31:28]];
    word[27:24]=S[word[27:24]];
    word[23:20]=S[word[23:20]];
    word[19:16]=S[word[19:16]];
    word[15:12]=S[word[15:12]];
    word[11:8]=S[word[11:8]];
    word[7:4]=S[word[7:4]];
    word[3:0]=S[word[3:0]];
    aux = word;
    for (i = 0; i < 64; i++) begin
      word[(i % 4) * 16 + i / 4] = aux[i];
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
  
  function void encript(inout logic[64-1:0] word, inout logic[80-1:0] key);
    for(cnt = 0; cnt < 31; cnt = cnt + 1) begin
      update_word(word, key[79:16]);
      update_key(key, cnt + 1);
    end
  endfunction

  virtual function void write_out(out_packet trans);
    stop_timer();  
    counter = 0;
    `uvm_info(get_type_name(), $sformatf("text:=%0h, key:=%0h", expected_text, expected_key), UVM_MEDIUM);
    encript(expected_text, expected_key);
    `uvm_info(get_type_name(), $sformatf("text:=%0h, key:=%0h", expected_text, expected_key), UVM_MEDIUM);
    if(trans.out != (expected_text ^ expected_key[79:16])) begin
      `uvm_fatal(get_type_name(),
                 $sformatf("Output data mismatch.  Expected : %0h.  Actual : %0h", 
                           (expected_text ^ expected_key[79:16]), trans.out))
      sbd_error = 1;
    end
    
  endfunction : write_out

  //    WHATCHDOG FUNCTIONS
  function void start_timer(int trigger_time);
    this.trigger_time   = trigger_time;
    this.start          = 1;
  endfunction : start_timer

  function void stop_timer();
    start           = 0;
  endfunction : stop_timer

  function integer get_time();
    return current_time;
  endfunction : get_time

endclass : scoreboard
