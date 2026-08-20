package dev.resurge.plugin;

import java.util.List;

public record ProbeResult(String format, double confidence, List<String> constraints) {
    public ProbeResult {
        if (confidence < 0 || confidence > 1) throw new IllegalArgumentException("confidence");
        constraints = List.copyOf(constraints);
    }
}
