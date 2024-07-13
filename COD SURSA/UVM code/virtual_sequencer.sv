class virtual_sequencer extends uvm_sequencer;
  `uvm_component_utils( virtual_sequencer )

  uvm_event reset;

  function new(string name = "virtual_sequencer", uvm_component = null);
    super.new(name, parent);
  endfunction

  function void build_phase(uvm_phase phase);
    if(!uvm_config_db#(uvm_event)::get(null,"","reset",reset))
       `uvm_fatal("NOVIF",{"reset evet must be set for: ",get_full_name(),".reset"});
  endfunction

  virtual task main_phase(uvm_phase phase);
    forever begin
      reset.wait_on();
      handle_reset(phase);
      reset.wait_off();
      start_phase_sequence(phase);
    end
  endtask 

  virtual function void handle_reset(uvm_phase phase);
      uvm_objection objection = phase.get_objection();
      uvm_object objectors[$];
      objection.get_objectors(objectors);
      foreach(objectors[i]) begin
        if(!uvm_re_match({get_full_name(),".*"},
                          objectors[i].get_full_name()))begin

          int objections_count = objection.get_objection_count(objectors[i]);

          if(objections_count > 0) begin
            objection.drop_objection(objectors[i], $sformatf("Dropping %0d objections at reset", objections_count), objections_count);
          end
        end
      end
      stop_sequences();
  endfunction

endclass
