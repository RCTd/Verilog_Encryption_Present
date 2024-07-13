class cmd_monitor extends uvm_monitor;

  protected virtual cmd_if cmd_vif;
  protected cmd_packet trans_collected;
  // The following two bits are used to control whether checks and coverage are
  // done both in the monitor class and the interface.
  bit checks_enable = 1;
  bit coverage_enable = 1;
  uvm_event reset;

  uvm_analysis_port#(cmd_packet) item_collected_port;

  `uvm_component_utils_begin(cmd_monitor)
    `uvm_field_int(checks_enable, UVM_DEFAULT)
    `uvm_field_int(coverage_enable, UVM_DEFAULT)
  `uvm_component_utils_end

  function new (string name, uvm_component parent);
    super.new(name, parent);
    trans_collected = new();
    item_collected_port = new("item_collected_port", this);
  endfunction : new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    if(!uvm_config_db#(virtual cmd_if)::get(this, "", "cmd_vif", cmd_vif))
       `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".cmd_vif"});
    if(!uvm_config_db#(uvm_event)::get(null,"","reset",reset))
       `uvm_fatal("NOVIF",{"reset evet must be set for: ",get_full_name(),".reset"});
  endfunction: build_phase


  virtual task run_phase(uvm_phase phase);
    `uvm_info(get_full_name(), "starting cmd monitor", UVM_MEDIUM);
    fork 
      collect_transactions();
      forever begin
        reset.wait_on();
        reset.wait_off();
      end
    join
  endtask : run_phase

  virtual protected task collect_transactions();
    forever begin
      @(posedge cmd_vif.clk)
      if(reset.is_off() && cmd_vif.pl)begin
        trans_collected.data_type = cmd_vif.pl[KEY] ? KEY : TEXT;
        trans_collected.data = cmd_vif.in;
        this.end_tr(trans_collected);
        end_collect_transaction();
      end

    end
  endtask : collect_transactions

  virtual protected function void end_collect_transaction();
    `uvm_info(get_full_name(), $sformatf("packet collected :\n%s",
      trans_collected.sprint()), UVM_MEDIUM)
    item_collected_port.write(trans_collected);
  endfunction : end_collect_transaction
    

endclass : cmd_monitor
