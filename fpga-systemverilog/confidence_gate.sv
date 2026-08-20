// Experimental FPGA preprocessing primitive. This is NOT a drive read-channel implementation.
// It only demonstrates a saturating confidence accumulator suitable for streamed observations.
module confidence_gate #(
    parameter WIDTH = 8
) (
    input  logic             clk,
    input  logic             rst_n,
    input  logic             valid,
    input  logic             observed_bit,
    output logic [WIDTH-1:0] score_zero,
    output logic [WIDTH-1:0] score_one
);
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            score_zero <= '0;
            score_one  <= '0;
        end else if (valid) begin
            if (observed_bit && score_one != {WIDTH{1'b1}}) score_one <= score_one + 1'b1;
            if (!observed_bit && score_zero != {WIDTH{1'b1}}) score_zero <= score_zero + 1'b1;
        end
    end
endmodule
