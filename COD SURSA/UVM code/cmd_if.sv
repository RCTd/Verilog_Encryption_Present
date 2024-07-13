interface cmd_if;

  // Control flags
  bit                has_checks = 1;
  bit                has_coverage = 1;

  // Actual Signals
  logic				 clk;
  logic [1 : 0]		 pl;
  logic [80-1 : 0]	 in;
// Coverage and assertions to be implemented here.
always @(negedge clk)
begin

// Valid must not be X or Z
assert_cmd_valid_unknown:assert property (
                  disable iff(!has_checks) 
  					!$isunknown(pl) )
                  else
                    $error("ERR_CMD_VALID_XZ\n cmd_valid went to X or Z");

// Oe must not be X or Z
assert_input_unknown:assert property (
                  disable iff(!has_checks) 
  (pl[0]|pl[1] |-> (!$isunknown(in)) ) )
                  else
                    $error("ERR_CMD_SIG\n Data went to X or Z during cmd_valid");

end

endinterface : cmd_if

