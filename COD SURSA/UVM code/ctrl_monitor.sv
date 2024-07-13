class ctrl_monitor extends uvm_monitor;

  protected virtual ctrl_if ctrl_vif;
  protected ctrl_packet trans_collected_reset;
  protected ctrl_packet trans_collected_oe;
  protected uvm_event reset;
  bit coverage_enable = 1;

  uvm_analysis_port#(ctrl_packet) item_collected_port;

  `uvm_component_utils_begin(ctrl_monitor)
    `uvm_field_int(coverage_enable, UVM_DEFAULT)
  `uvm_component_utils_end

  function new (string name, uvm_component parent);
    super.new(name, parent);
    cov_ctrl_trans = new();
    cov_ctrl_trans.set_inst_name({get_full_name(), ".cov_ctrl_trans"});
    trans_collected_reset   = new();
    trans_collected_oe      = new();
    item_collected_port = new("item_collected_port", this);
  endfunction : new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    if(!uvm_config_db#(virtual ctrl_if)::get(this, "", "ctrl_vif", ctrl_vif))
       `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".ctrl_vif"});
    if(!uvm_config_db#(uvm_event)::get(null,"","reset",reset))
       `uvm_fatal("NOVIF",{"reset evet must be set for: ",get_full_name(),".reset"});
  endfunction: build_phase


  virtual task run_phase(uvm_phase phase);
    `uvm_info(get_full_name(), "starting ctrl monitor", UVM_MEDIUM);
    fork 
      collect_transactions();
      detect_reset();
    join
  endtask : run_phase

  virtual protected task collect_transactions();
      forever begin
        @(ctrl_vif.reset);
        void'(this.begin_tr(trans_collected_reset));
        trans_collected_reset.data      = ctrl_vif.reset;
        this.end_tr(trans_collected_reset);

        `uvm_info(get_full_name(), $sformatf("packet collected :\n%s",
          trans_collected_reset.sprint()), UVM_FULL)
        if (coverage_enable)
          perform_packet_coverage();
        item_collected_port.write(trans_collected_reset);
      end
  endtask : collect_transactions

  virtual protected task detect_reset();
    fork
      forever begin
        @(posedge ctrl_vif.reset == ACTIVE_RESET);
        reset.trigger();
      end
      forever begin
        @(negedge ctrl_vif.reset == ACTIVE_RESET);
        reset.reset();
      end
    join
  endtask : detect_reset


  covergroup cov_ctrl_trans;
    option.per_instance = 1;
    trans_reset : coverpoint trans_collected_reset.data;
  endgroup : cov_ctrl_trans

  virtual protected function void perform_packet_coverage();
    cov_ctrl_trans.sample();
  endfunction : perform_packet_coverage

  virtual function void report_phase(uvm_phase phase);
    `uvm_info(get_full_name(),$sformatf("Covergroup 'cov_ctrl_trans' coverage: %2f",
					cov_ctrl_trans.get_inst_coverage()),UVM_LOW)
  endfunction

endclass : ctrl_monitor
