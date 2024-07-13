//------------------------------------------------------------------------------
//
// SEQUENCE: base_cmd_sequence
//
//------------------------------------------------------------------------------

// This sequence raises/drops objections in the pre/post_body so that root
// sequences raise objections but subsequences do not.

virtual class base_cmd_sequence extends uvm_sequence #(cmd_transfer);

  function new(string name="base_cmd_seq");
     super.new(name);
     set_automatic_phase_objection(1);
  endfunction
  
endclass : base_cmd_sequence


//------------------------------------------------------------------------------
//
// SEQUENCE: start_phase
//
//------------------------------------------------------------------------------

// This sequence starts with a specific pwm setting then continues with random seq

class start_phase extends base_cmd_sequence;
  
  int unsigned text, key;

  function new(string name = "start_phase");
    super.new(name);
  endfunction : new

  `uvm_object_utils(start_phase)

  virtual task body();
    void'(uvm_config_db#(int unsigned)::get(null,get_full_name(),"text",      text));
    void'(uvm_config_db#(int unsigned)::get(null,get_full_name(),"key",        key));

    `uvm_info(get_type_name(),
              $sformatf("%s starting... with text = %0h, key = %0h",
      get_sequence_path(), text, key), UVM_MEDIUM);

    
    `uvm_do_with(req,
                 {data_type == KEY;
                  data == key;
                  transmit_delay == 2;} )
    get_response(rsp);
    `uvm_info(get_type_name(),
              $sformatf("%s set key = %0d",
      get_sequence_path(), rsp.data), UVM_MEDIUM);
    
    `uvm_do_with(req,
                 {data_type == TEXT;
                  data == text;
                  transmit_delay == 40;} )
    get_response(rsp);
    `uvm_info(get_type_name(),
              $sformatf("%s set text = %0d",
      get_sequence_path(), rsp.data), UVM_MEDIUM);
    
    

    repeat(10) begin
      `uvm_do_with(req, {data_type dist {TEXT := 3, KEY := 1};
                         transmit_delay inside {[40:60]};})
      get_response(rsp);
    end
    
    repeat(200) begin
      `uvm_do_with(req, {data_type dist {TEXT := 3, KEY := 1};
        				 transmit_delay inside {[0:40]};})
      get_response(rsp);
    end
  endtask : body
    
endclass : start_phase