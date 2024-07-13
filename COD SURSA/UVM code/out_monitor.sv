class out_monitor extends uvm_monitor;

  protected virtual out_if out_vif;
  protected out_packet trans_collected;
  bit coverage_enable = 1;

  uvm_analysis_port#(out_packet) item_collected_port;

  `uvm_component_utils_begin(out_monitor)
    `uvm_field_int(coverage_enable, UVM_DEFAULT)
  `uvm_component_utils_end

  function new (string name, uvm_component parent);
    super.new(name, parent);
    cov_out_trans = new();
    cov_out_trans.set_inst_name({get_full_name(), ".cov_out_trans"});
    trans_collected = new();
    item_collected_port = new("item_collected_port", this);
  endfunction : new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    if(!uvm_config_db#(virtual out_if)::get(this, "", "out_vif", out_vif))
       `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".out_vif"});
  endfunction: build_phase


  virtual task run_phase(uvm_phase phase);
    `uvm_info(get_full_name(), "starting out monitor", UVM_MEDIUM);
    fork 
      collect_transactions();
    join
  endtask : run_phase

  virtual protected task collect_transactions();
    forever begin
      @(posedge out_vif.done);
      @(negedge out_vif.clk);
      void'(this.begin_tr(trans_collected));
      trans_collected.out = out_vif.out;
      this.end_tr(trans_collected);

      `uvm_info(get_full_name(), $sformatf("packet collected :\n%s",
        trans_collected.sprint()), UVM_MEDIUM)
      if (coverage_enable)
        perform_packet_coverage();
      item_collected_port.write(trans_collected);
    end
  endtask : collect_transactions

  covergroup cov_out_trans;
    option.per_instance = 1;
    trans_cmd_out     : coverpoint trans_collected.out; 
  endgroup : cov_out_trans

  virtual protected function void perform_packet_coverage();
    cov_out_trans.sample();
  endfunction : perform_packet_coverage

  virtual function void report_phase(uvm_phase phase);
    `uvm_info(get_full_name(),$sformatf("Covergroup 'cov_out_trans' coverage: %2f",
					cov_out_trans.get_inst_coverage()),UVM_LOW)
  endfunction
endclass : out_monitor