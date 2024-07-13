interface out_if;

  // Control flags
  bit                has_checks = 1;
  bit                has_coverage = 1;

  // Actual Signals
 logic              clk;
  logic [64-1:0] 	 out;
  logic 			 done;

endinterface : out_if

